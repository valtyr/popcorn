#pragma once

#include "io.h"
#include "isr.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

typedef struct PS2KeyboardEvent
{
    char character;
    int keycode;
    bool shiftPressed;
    bool ctrlPressed;
    bool altPressed;
    bool altGrPressed;
    bool metaPressed;
} PS2KeyboardEvent;
typedef void (*PS2KeyboardEventHandler)(PS2KeyboardEvent);

void PS2Init();