#pragma once

#include <stddef.h>
#include "types.h"

typedef struct __attribute__((__packed__)) FBPixel
{
    u8 b;
    u8 g;
    u8 r;
    u8 a;
} FBPixel;

typedef struct __attribute__((__packed__)) FBImage
{
    u32 width;
    u32 height;
    FBPixel pixels[];
} FBImage;

typedef struct FBInfo
{
    u32 width;
    u32 height;
    FBPixel *physicalBuffer;
} FBInfo;

static FBPixel FBhsv(double h, double s, double v);

void FBInit(int width, int height, void *address);
void FBOutput();
void FBSet(int x, int y, FBPixel pixel);
FBPixel FBGet(int x, int y);

FBImage FBReadFBI(u8 *location);
void FBBlitPixel(int x, int y, FBPixel sourcePixel);
void FBBlit(int x, int y, FBImage *image);
void FBFill(u8 r, u8 g, u8 b);

void FBDisplayTestPattern();
