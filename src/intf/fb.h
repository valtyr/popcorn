#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct __attribute__((__packed__)) FBPixel
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
} FBPixel;

typedef struct __attribute__((__packed__)) FBImage
{
    uint32_t width;
    uint32_t height;
    FBPixel pixels[];
} FBImage;

typedef struct FBInfo
{
    uint32_t width;
    uint32_t height;
    FBPixel *physicalBuffer;
} FBInfo;

static FBPixel FBhsv(double h, double s, double v);

void FBInit(int width, int height, void *address);
void FBOutput();
void FBSet(int x, int y, FBPixel pixel);
FBPixel FBGet(int x, int y);

FBImage FBReadFBI(uint8_t *location);
void FBBlitPixel(int x, int y, FBPixel sourcePixel);
void FBBlit(int x, int y, FBImage *image);
void FBFill(uint8_t r, uint8_t g, uint8_t b);

void FBDisplayTestPattern();
