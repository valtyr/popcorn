#include "fb.h"

#define atan __builtin_atan
#define round __builtin_round

static FBInfo framebufferInfo;

extern FBPixel video_back_buffer[];

static inline void CopyMemory(void *dst, const void *src, size_t size)
{
    __asm__ __volatile__("rep movsb"
                         : "+D"(dst), "+S"(src), "+c"(size)
                         :
                         : "memory");
}

void FBInit(int width, int height, void *address)
{
    framebufferInfo.width = width;
    framebufferInfo.height = height;
    framebufferInfo.physicalBuffer = address;
}

void FBOutput()
{
    CopyMemory(
        framebufferInfo.physicalBuffer,
        video_back_buffer,
        framebufferInfo.width * framebufferInfo.height * 4);
}

void FBSet(int x, int y, FBPixel pixel)
{
    uint32_t pixelOffset = y * framebufferInfo.width + x;
    video_back_buffer[pixelOffset] = pixel;
    // video_back_buffer[x + y * framebufferInfo.height] = pixel;
}

FBPixel FBGet(int x, int y)
{
    uint32_t pixelOffset = y * framebufferInfo.width + x;
    return video_back_buffer[pixelOffset];
}

void FBDisplayTestPattern()
{
    for (int x = 0; x < framebufferInfo.width; x++)
    {
        for (int y = 0; y < framebufferInfo.height; y++)
        {
            FBSet(x, y, (FBPixel){.r = (x / (framebufferInfo.width * 1.0)) * 255, .g = (y / (framebufferInfo.height * 1.0)) * 255, .b = 128});
        }
    }
}

// Causes crash for some reason,
// probably best to move all of
// of the blitting code to user-
// space and forget about any
// "complex" math functions in
// the kernel.
//
// Let's not pretend I'm any good
// at implementing math primitives.
int naiveRound(double d)
{
    int floor = (int)d;
    if (d - floor >= 0.5)
        return floor + 1;
    return floor;
}

// I think this function can not
// return a 255 value for any of
// the color components because
// the double arithmetic is simply
// floored.
void FBBlitPixel(int x, int y, FBPixel sourcePixel)
{
    if (sourcePixel.a == 255)
    {
        FBSet(x, y, sourcePixel);
        return;
    }

    if (sourcePixel.a == 0)
    {
        return;
    }

    FBPixel destinationPixel = FBGet(x, y);
    FBPixel newPixel = {
        .r = sourcePixel.r * (sourcePixel.a / 255.0) + destinationPixel.r * (1.0 - sourcePixel.a / 255.0),
        .g = sourcePixel.g * (sourcePixel.a / 255.0) + destinationPixel.g * (1.0 - sourcePixel.a / 255.0),
        .b = sourcePixel.b * (sourcePixel.a / 255.0) + destinationPixel.b * (1.0 - sourcePixel.a / 255.0),
        .a = 255};

    FBSet(x, y, newPixel);
}

void FBBlit(int x, int y, FBImage *image)
{
    for (int xi = 0; xi < image->width && xi < framebufferInfo.width - x; xi++)
    {
        for (int yi = 0; yi < image->height && yi < framebufferInfo.height - y; yi++)
        {
            FBBlitPixel(x + xi, y + yi, image->pixels[xi + yi * image->width]);
        }
    }
}

void FBFill(uint8_t r, uint8_t g, uint8_t b)
{
    for (int x = 0; x < framebufferInfo.width; x++)
    {
        for (int y = 0; y < framebufferInfo.height; y++)
        {
            FBBlitPixel(x, y, (FBPixel){.r = r, .g = g, .b = b, .a = 255});
        }
    }
}