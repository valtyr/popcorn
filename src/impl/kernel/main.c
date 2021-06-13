#include "bios.h"
#include "multiboot2.h"
#include "fb.h"

MultibootInfo systemInfo;

extern FBImage popcorn_logo;

void kernel_main(uint32_t magic, void *addr)
{
    BIOSGreeting();

    BIOSPrintf("Magic: 0x%x; Multiboot info address: 0x%x;\n", magic, addr);

    uint32_t *multibootResponse = addr;
    int length = 1;

    while (!(multibootResponse[-1] == 0 && multibootResponse[length] == 8))
        length++;

    BIOSHexdump(multibootResponse, (length + 1) * 4);

    // Parse Multiboot 2 response into systemInfo
    Multiboot2GetSystemInfo(magic, addr, &systemInfo);
    Multiboot2PrintResponse(&systemInfo);

    // Initialize framebuffer and display greeting
    FBInit(systemInfo.framebufferWidth, systemInfo.framebufferHeight, (void *)systemInfo.framebufferAddress);
    FBDisplayTestPattern();
    FBBlit(systemInfo.framebufferWidth / 2 - popcorn_logo.width / 2, systemInfo.framebufferHeight / 2 - popcorn_logo.height / 2, &popcorn_logo);
    FBOutput();

    BIOSHalt();
}
