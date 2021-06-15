#include "ps2.h"

static bool initialized = false;

static volatile uint8_t shift_pressed = 0;

static const uint8_t keyboard_map[] =
    {
        0,
        27, // Escape
        '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
        '\n', // Enter
        0,    // Ctrl
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
        0,   // Shift
        0,   // Print Scrn
        0,   // Alt
        ' ', // Spacebar
};

static const uint8_t shifted_keyboard_map[] =
    {
        0,
        27, // Escape
        '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
        '\n', // Enter
        0,    // Ctrl
        'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~', 0, '|',
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',
        0,   // Shift
        0,   // Print Scrn
        0,   // Alt
        ' ', // Spacebar
};

static void interrupt_handler(registers_t registers)
{

    if (inb(KEYBOARD_STATUS_PORT) & 0x01)
    {
        uint8_t keycode = inb(KEYBOARD_DATA_PORT);

        // Switch to shifted keyboard map when shift is pressed
        if (shift_pressed == 0 && (keycode == 42 || keycode == 54))
        {
            shift_pressed = keycode;
        }
        // Release shifted keyboard map if shift was relieved
        if ((shift_pressed == 42 && keycode == 170) || (shift_pressed == 54 && keycode == 182))
        {
            shift_pressed = 0;
        }
        if (keycode > sizeof(keyboard_map))
        {
            return;
        }
        shift_pressed > 0 ? BIOSPrintf("%c", shifted_keyboard_map[keycode]) : BIOSPrintf("%c", keyboard_map[keycode]);
    }
}

void PS2Init()
{
    if (initialized)
        return;

    ISRRegisterHandler(33, interrupt_handler); // Register keyboard handler should replace with const

    initialized = true;
}