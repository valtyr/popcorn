#include "multiboot2.h"

static char *tagNames[];

#define MB2_GET_VALUE(type) *(type *)((responsePointer + (accumulatedSize += sizeof(type)) - sizeof(type)))

void Multiboot2GetSystemInfo(u32 magic, void *responseAddress, MultibootInfo *systemInfo)
{
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
        BIOSPanic("Invalid Multiboot magic");

    systemInfo->valid = true;
    systemInfo->responseAddress = (u32)responseAddress;

    void *responsePointer = responseAddress;

    u32 totalHeaderSize = *(u32 *)responsePointer;
    u32 reserved = *(u32 *)(responsePointer + 4);
    systemInfo->totalHeaderSize = totalHeaderSize;
    responsePointer += 8;

    if (reserved != 0)
        BIOSPanic("Multiboot reserved field should be 0");

    // Make sure tags start at 8-byte aligned boundaries
    // when incrementing the responsePointer
    while (responsePointer < responseAddress + totalHeaderSize)
    {
        u32 type = *(u32 *)responsePointer;
        u32 size = *(u32 *)(responsePointer + 4);

        int accumulatedSize = 8;

        // Break early if end tag is reached.
        if (type == 0 && size == 8)
            break;

        switch (type)
        {
        case MULTIBOOT2_TAG_CMD_LINE:
            systemInfo->commandLineString = (char *)(responsePointer + 8);
            break;
        case MULTIBOOT2_TAG_BASIC_MEMORY:
            systemInfo->lowerMemoryAmount = MB2_GET_VALUE(u32);
            systemInfo->upperMemoryAmount = MB2_GET_VALUE(u32);
            break;
        case MULTIBOOT2_TAG_BOOT_DEVICE:
            systemInfo->bootDeviceNumber = MB2_GET_VALUE(u32);
            systemInfo->bootDevicePartition = MB2_GET_VALUE(u32);
            systemInfo->bootDeviceSubpartition = MB2_GET_VALUE(u32);
            break;
        case MULTIBOOT2_TAG_FRAMEBUFFER:
            systemInfo->framebufferAddress = MB2_GET_VALUE(u64);
            systemInfo->framebufferPitch = MB2_GET_VALUE(u32);
            systemInfo->framebufferWidth = MB2_GET_VALUE(u32);
            systemInfo->framebufferHeight = MB2_GET_VALUE(u32);
            systemInfo->framebufferBits = MB2_GET_VALUE(u8);
            systemInfo->framebufferType = MB2_GET_VALUE(u8);
            // Should assert reserved keyword is 0;
            // *(u8*) (responsePointer + 27) = 0;
            break;
        default:
            // Use when implementing new tags
            // BIOSPrintf("Multiboot tag of type %s (%d) not handled.\n", tagNames[type], type);
            break;
        }

        responsePointer += size;
        responsePointer += ((u32)responseAddress - (u32)responsePointer) % 8;
    }
}

void Multiboot2PrintResponse(MultibootInfo *systemInfo)
{
    BIOSPrint("\n┌──  Multiboot 2 response\n");
    BIOSPrint("│ \n");
    BIOSPrintf("│  Total header size: %d;\n", systemInfo->totalHeaderSize);
    BIOSPrintf("│  Command line string: \"%s\";\n", systemInfo->commandLineString);
    BIOSPrintf("│  Lower memory: %dKB; Upper memory: %dKB;\n", systemInfo->lowerMemoryAmount, systemInfo->upperMemoryAmount);
    BIOSPrintf("│  Boot device: 0x%x; Boot device partition: %d; Boot device subpartition %d;\n", systemInfo->bootDeviceNumber, systemInfo->bootDevicePartition, systemInfo->bootDeviceSubpartition);
    BIOSPrintf(
        "│  FBAddress: 0x%x; FBPitch: %d; FBWidth %d; FBHeight %d; FBBits %d;  FBType %d;\n",
        systemInfo->framebufferAddress,
        systemInfo->framebufferPitch,
        systemInfo->framebufferWidth,
        systemInfo->framebufferHeight,
        systemInfo->framebufferBits,
        systemInfo->framebufferType);
    BIOSPrint("│ \n");
    BIOSPrint("└── \n");
}

static char *tagNames[] = {
    /*  0 */ "STOP",
    /*  1 */ "Boot command line",
    /*  2 */ "Boot loader name",
    /*  3 */ "Modules",
    /*  4 */ "Basic memory information",
    /*  5 */ "BIOS Boot device",
    /*  6 */ "Memory map",
    /*  7 */ "VBE info",
    /*  8 */ "Framebuffer info",
    /*  9 */ "ELF-Symbols",
    /* 10 */ "APM table",
    /* 11 */ "EFI 32-bit system table pointer",
    /* 12 */ "EFI 64-bit system table pointer",
    /* 13 */ "SMBIOS tables",
    /* 14 */ "ACPI old RSDP",
    /* 15 */ "ACPI new RSDP",
    /* 16 */ "Networking information",
    /* 17 */ "EFI memory map",
    /* 18 */ "EFI boot services not terminated",
    /* 19 */ "EFI 32-bit image handle pointer",
    /* 20 */ "EFI 64-bit image handle pointer",
    /* 21 */ "Image load base physical address",
};
