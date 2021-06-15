#include "bios.h"
#include "multiboot2.h"
#include "fb.h"
#include "dt.h"

MultibootInfo systemInfo;

extern FBImage popcorn_logo;

void kernel_main(u32 magic, void *addr)
{
    BIOSGreeting();

    BIOSPrintf("Magic: 0x%x; Multiboot info address: 0x%x;\n", magic, addr);

    // Parse Multiboot 2 response into systemInfo
    Multiboot2GetSystemInfo(magic, addr, &systemInfo);
    Multiboot2PrintResponse(&systemInfo);

    // Initialize descriptor tables
    InitDescriptorTables();

    // Initialize framebuffer and display greeting
    FBInit(systemInfo.framebufferWidth, systemInfo.framebufferHeight, (void *)systemInfo.framebufferAddress);
    FBDisplayTestPattern();
    FBBlit(systemInfo.framebufferWidth / 2 - popcorn_logo.width / 2, systemInfo.framebufferHeight / 2 - popcorn_logo.height / 2, &popcorn_logo);
    FBOutput();

    // PS2Init();

    // while (true)
    BIOSHalt();
}
