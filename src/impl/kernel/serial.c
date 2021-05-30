#include "serial.h"

// Borrowed from SerenityOS

void SerialPutch(char ch)
{
    static bool serial_ready = false;
    static bool was_cr = false;

    if (!serial_ready)
    {
        outb(0x3F8 + 1, 0x00);
        outb(0x3F8 + 3, 0x80);
        outb(0x3F8 + 0, 0x02);
        outb(0x3F8 + 1, 0x00);
        outb(0x3F8 + 3, 0x03);
        outb(0x3F8 + 2, 0xC7);
        outb(0x3F8 + 4, 0x0B);

        serial_ready = true;
    }

    while ((inb(0x3F8 + 5) & 0x20) == 0)
    {
    }

    if (ch == '\n' && !was_cr)
        outb(0x3F8, '\r');

    outb(0x3F8, ch);

    if (ch == '\r')
        was_cr = true;
    else
        was_cr = false;
}
