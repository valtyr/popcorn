#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "io.h"
#include "serial.h"

typedef enum
{
    BIOS_COLOR_BLACK = 0,
    BIOS_COLOR_BLUE = 1,
    BIOS_COLOR_GREEN = 2,
    BIOS_COLOR_CYAN = 3,
    BIOS_COLOR_RED = 4,
    BIOS_COLOR_MAGENTA = 5,
    BIOS_COLOR_BROWN = 6,
    BIOS_COLOR_LIGHT_GRAY = 7,
    BIOS_COLOR_DARK_GRAY = 8,
    BIOS_COLOR_LIGHT_BLUE = 9,
    BIOS_COLOR_LIGHT_GREEN = 10,
    BIOS_COLOR_LIGHT_CYAN = 11,
    BIOS_COLOR_LIGHT_RED = 12,
    BIOS_COLOR_PINK = 13,
    BIOS_COLOR_YELLOW = 14,
    BIOS_COLOR_WHITE = 15,
} BIOSColor;

void BIOSClear();
void BIOSPrintChar(char);
void BIOSPrint(char *);
void BIOSPrintBlink(char *);
void BIOSSetColor(BIOSColor foreground, BIOSColor background);
void BIOSPanic(char *message);
void BIOSPrintf(const char *format, ...);
void BIOSHexdump(uint8_t *address, size_t length);
void BIOSGreeting();

void BIOSHalt();

extern void BIOSWait(uint64_t microseconds);

#define ASSERT(condition, reason) (                                                \
    {                                                                              \
        if (!(condition))                                                          \
        {                                                                          \
            BIOSSetColor(BIOS_COLOR_WHITE, BIOS_COLOR_RED);                        \
            BIOSPrintf("\nPanic! Assertion error at %s:%d\n", __FILE__, __LINE__); \
            BIOSPrintf("Reason: %s", reason);                                      \
            BIOSHalt();                                                            \
        }                                                                          \
    })
