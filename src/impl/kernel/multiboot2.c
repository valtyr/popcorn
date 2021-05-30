#include "multiboot2.h"

static char *tagNames[];

void Multiboot2GetSystemInfo(uint32_t magic, void *responseAddress, MultibootInfo *systemInfo)
{
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
        BIOSPanic("Invalid Multiboot magic");

    systemInfo->valid = true;
    systemInfo->responseAddress = (uint32_t)responseAddress;

    void *responsePointer = responseAddress;

    uint32_t totalHeaderSize = *(uint32_t *)responsePointer;
    uint32_t reserved = *(uint32_t *)(responsePointer + 4);
    systemInfo->totalHeaderSize = totalHeaderSize;
    responsePointer += 8;

    if (reserved != 0)
        BIOSPanic("Multiboot reserved field should be 0");

    // Make sure tags start at 8-byte aligned boundaries
    // when incrementing the responsePointer
    while (responsePointer < responseAddress + totalHeaderSize)
    {
        uint32_t type = *(uint32_t *)responsePointer;
        uint32_t size = *(uint32_t *)(responsePointer + 4);

        // Break early if end tag is reached.
        if (type == 0 && size == 8)
            break;

        switch (type)
        {
        case MULTIBOOT2_TAG_CMD_LINE:
            systemInfo->commandLineString = (char *)(responsePointer + 8);
            break;
        case MULTIBOOT2_TAG_BASIC_MEMORY:
            systemInfo->lowerMemoryAmount = *(uint32_t *)(responsePointer + 8);
            systemInfo->upperMemoryAmount = *(uint32_t *)(responsePointer + 12);
            break;
        case MULTIBOOT2_TAG_BOOT_DEVICE:
            systemInfo->bootDeviceNumber = *(uint32_t *)(responsePointer + 8);
            systemInfo->bootDevicePartition = *(uint32_t *)(responsePointer + 12);
            systemInfo->bootDeviceSubpartition = *(uint32_t *)(responsePointer + 16);
            break;
        case MULTIBOOT2_TAG_FRAMEBUFFER:
            systemInfo->framebufferAddress = *(uint64_t *)(responsePointer + 8);
            systemInfo->framebufferPitch = *(uint32_t *)(responsePointer + 16);
            systemInfo->framebufferWidth = *(uint32_t *)(responsePointer + 20);
            systemInfo->framebufferHeight = *(uint32_t *)(responsePointer + 24);
            systemInfo->framebufferBits = *(int8_t *)(responsePointer + 25);
            systemInfo->framebufferType = *(int8_t *)(responsePointer + 26);
            // Should assert reserved keyword is 0;
            // *(uint8_t*) (responsePointer + 27) = 0;
            break;
        default:
            // BIOSPrintf("Multiboot tag of type %s (%d) not handled.\n", tagNames[type], type);
            break;
        }

        responsePointer += size;
        responsePointer += ((uint32_t)responseAddress - (uint32_t)responsePointer) & 0x7;
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
