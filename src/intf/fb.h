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

typedef struct FBInfo
{
    int width;
    int height;
    FBPixel *physicalBuffer;
} FBInfo;

static FBPixel FBhsv(double h, double s, double v);

void FBInit(int width, int height, void *address);
void FBOutput();
void FBSet(int x, int y, FBPixel pixel);
FBPixel FBGet(int x, int y);

void FBDisplayTestPattern();
