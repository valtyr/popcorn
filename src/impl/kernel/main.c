#include "bios.h"
#include "multiboot2.h"

MultibootInfo systemInfo;


void kernel_main(uint32_t magic, void* addr) {

    BIOSClear();

    BIOSSetColor(BIOS_COLOR_BLACK, BIOS_COLOR_YELLOW);
    BIOSPrint("Popcorn kernel \2 attempting to boot.\n");
    BIOSSetColor(BIOS_COLOR_WHITE, BIOS_COLOR_BLACK);

    BIOSPrintf("Magic: 0x%x; Multiboot info address: 0x%x;\n", magic, addr);

    // Parse Multiboot info
    switch(getSystemInfo(magic, addr, &systemInfo)) {
    case MULTIBOOT2_OK:
        break;
    case MULTIBOOT2_INVALID_MAGIC:
        BIOSPanic("Invalid Multiboot magic");
        break;
    case MULTIBOOT2_INVALID_RESERVED_FIELD:
        BIOSPanic("Multiboot reserved field should be 0");
        break;
    default:
        break;
    }

    BIOSPrintf("Total header size: %d;\n", systemInfo.totalHeaderSize);
    BIOSPrintf("Command line string: \"%s\";\n", systemInfo.commandLineString);
    BIOSPrintf("Lower memory: %dKB; Upper memory: %dKB;\n", systemInfo.lowerMemoryAmount, systemInfo.upperMemoryAmount);
    BIOSPrintf("Boot device: 0x%x; Boot device partition: %d; Boot device subpartition %d;\n", systemInfo.bootDeviceNumber, systemInfo.bootDevicePartition, systemInfo.bootDeviceSubpartition);
    BIOSPrintf(
        "FBAddress: 0x%x; FBPitch: %d; FBWidth %d; FBHeight %d; FBBits %d;  FBType %d;\n",
        systemInfo.framebufferAddress,
        systemInfo.framebufferPitch,
        systemInfo.framebufferWidth,
        systemInfo.framebufferHeight,
        systemInfo.framebufferBits,
        systemInfo.framebufferType
    );

    BIOSHexdump(addr, 128);

    BIOSHalt();
}
