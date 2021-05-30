#include "bios.h"
#include "multiboot2.h"
#include "fb.h"

MultibootInfo systemInfo;

void kernel_main(uint32_t magic, void *addr)
{
    BIOSGreeting();

    BIOSPrintf("Magic: 0x%x; Multiboot info address: 0x%x;\n", magic, addr);

    Multiboot2GetSystemInfo(magic, addr, &systemInfo);
    Multiboot2PrintResponse(&systemInfo);

    FBInit(systemInfo.framebufferWidth, systemInfo.framebufferHeight, (void *)systemInfo.framebufferAddress);

    FBDisplayTestPattern();
    FBOutput();

    BIOSHalt();
}
