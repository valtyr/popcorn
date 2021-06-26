#include "bios.h"
#include "multiboot2.h"
#include "fb.h"
#include "dt.h"
#include "ps2.h"
#include "rtc.h"
#include "pit.h"
#include "sb16.h"

MultibootInfo systemInfo;

extern FBImage popcorn_logo;

extern u16 popcorn_chime[];
extern u32 popcorn_chime_length;

static u8 s_pit_counter = 0;
static u8 s_swap_counter = 0;

void kernel_main(u32 magic, void *addr)
{
    BIOSGreeting();

    BIOSPrintf("Magic: 0x%x; Multiboot info address: 0x%x;\n", magic, addr);

    // Parse Multiboot 2 response into systemInfo
    Multiboot2GetSystemInfo(magic, addr, &systemInfo);
    Multiboot2PrintResponse(&systemInfo);

    // Initialize descriptor tables
    InitDescriptorTables();

    RTCInit();
    PITInit();

    // Initialize framebuffer and display greeting
    FBInit(systemInfo.framebufferWidth, systemInfo.framebufferHeight, (void *)systemInfo.framebufferAddress);
    FBDisplayTestPattern();
    FBBlit(systemInfo.framebufferWidth / 2 - popcorn_logo.width / 2, systemInfo.framebufferHeight / 2 - popcorn_logo.height / 2, &popcorn_logo);
    FBOutput();

    // As of right now this shuts down the emulator
    // when ESC is pressed.
    PS2Init();

    SB16Init();
    // SB16DirectPlay(popcorn_chime, popcorn_chime_length);

    while (true)
    {
        __asm__("nop");
    }
}
