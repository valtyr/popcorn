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
    return video_back_buffer[x + y * framebufferInfo.height];
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
