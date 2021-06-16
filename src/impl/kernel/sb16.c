#include "sb16.h"

#define DSP_READ 0x22A
#define DSP_WRITE 0x22C
#define DSP_STATUS 0x22E
#define DSP_R_ACK 0x22F

typedef enum
{
    SampleFormatSigned = 0x10,
    SampleFormatStereo = 0x20,
} SampleFormat;

void dsp_write(u8 value)
{
    while (inb(DSP_WRITE) & 0x80)
    {
        // Chill
    }
    outb(DSP_WRITE, value);
}

u8 dsp_read()
{
    while (!(inb(DSP_STATUS) & 0x80))
    {
        // Chill
    }
    return inb(DSP_READ);
}

void set_sample_rate(u16 hz)
{
    dsp_write(0x41); // output
    dsp_write((u8)(hz >> 8));
    dsp_write((u8)hz);
    dsp_write(0x42); // input
    dsp_write((u8)(hz >> 8));
    dsp_write((u8)hz);
}

void SB16Init()
{
    // Disable interrupts
    cli();

    outb(0x226, 1);
    delay(32);
    outb(0x226, 0);

    u8 data = dsp_read();
    if (data != 0xAA)
    {
        BIOSPrintf("Soundblaster not ready :(");
        return;
    }

    // Get the version info
    dsp_write(0xe1);
    u8 majorVersion = dsp_read();
    u8 minorVersion = dsp_read();

    sti();

    BIOSPrintf("SB16: Found version %d.%d", majorVersion, minorVersion);

    // set_irq_register(SB16_DEFAULT_IRQ);
    // dmesgln("SB16: IRQ {}", get_irq_line());
}

void SB16DirectPlay(u16 *buffer, u32 length)
{

    const int sample_rate = 12000;
    set_sample_rate(sample_rate);

    const u8 channel = 5; // 16-bit samples use DMA channel 5 (on the master DMA controller)
    const u8 mode = 0x48;
    const u32 addr = (u32)buffer;

    // Disable the DMA channel
    outb(0xd4, 4 + (channel % 4));

    // Clear the byte pointer flip-flop
    outb(0xd8, 0);

    // Write the DMA mode for the transfer
    outb(0xd6, (channel % 4) | mode);

    // Write the offset of the buffer
    u16 offset = (addr / 2) % 65536;
    outb(0xc4, (u8)offset);
    outb(0xc4, (u8)(offset >> 8));

    // Write the transfer length
    outb(0xc6, (u8)(length - 1));
    outb(0xc6, (u8)((length - 1) >> 8));

    // Write the buffer
    outb(0x8b, addr >> 16);

    // Enable the DMA channel
    outb(0xd4, (channel % 4));

    u8 command = 0xb0;
    u8 sampleMode = (u8)SampleFormatSigned | (u8)SampleFormatStereo;

    u16 sample_count = length / sizeof(u16);
    if (mode & (u8)SampleFormatStereo)
        sample_count /= 2;

    sample_count -= 1;

    // cli();
    // enable_irq();

    dsp_write(command);
    dsp_write(sampleMode);
    dsp_write((u8)sample_count);
    dsp_write((u8)(sample_count >> 8));
}
