#include "ps2.h"

static void interrupt_handler(registers_t registers)
{

    if (inb(KEYBOARD_STATUS_PORT) & 0x01)
    {
        uint8_t keycode = inb(KEYBOARD_DATA_PORT);

        BIOSPrintf("PS2 Interrupt called with keycode %d\n", keycode);
    }
}

void PS2Init()
{
    ISRRegisterHandler(33, interrupt_handler); // Register keyboard handler should replace with const
}