#include "multiboot2.h"
#include "bios.h"

static char* tagNames[];

int getSystemInfo(uint32_t magic, void* responseAddress, MultibootInfo* systemInfo)
{
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
        return MULTIBOOT2_INVALID_MAGIC;

    systemInfo->valid = true;
    systemInfo->responseAddress = (uint32_t) responseAddress;

    void* responsePointer = responseAddress;
    
    uint32_t totalHeaderSize = *(uint32_t*) responsePointer;
    uint32_t reserved = *(uint32_t*) (responsePointer + 4);
    systemInfo->totalHeaderSize = totalHeaderSize;
    responsePointer += 8;

    if (reserved != 0)
        return MULTIBOOT2_INVALID_RESERVED_FIELD;
    
    // Make sure tags start at 8-byte aligned boundaries
    // when incrementing the responsePointer
    while(responsePointer < responseAddress + totalHeaderSize) {
        uint32_t type = *(uint32_t*) responsePointer;
        uint32_t size = *(uint32_t*) (responsePointer + 4);



        // Break early if end tag is reached.
        if (type == 0 && size == 8)
            break;


        switch(type){
        case MULTIBOOT2_TAG_CMD_LINE:
            systemInfo->commandLineString = (char*) (responsePointer + 8);
            break;
        case MULTIBOOT2_TAG_BASIC_MEMORY:
            systemInfo->lowerMemoryAmount = *(uint32_t*) (responsePointer + 8);
            systemInfo->upperMemoryAmount = *(uint32_t*) (responsePointer + 12);
            break;
        case MULTIBOOT2_TAG_BOOT_DEVICE:
            systemInfo->bootDeviceNumber = *(uint32_t*) (responsePointer + 8);
            systemInfo->bootDevicePartition = *(uint32_t*) (responsePointer + 12);
            systemInfo->bootDeviceSubpartition = *(uint32_t*) (responsePointer + 16);
            break;
        case MULTIBOOT2_TAG_FRAMEBUFFER:
            systemInfo->framebufferAddress = *(uint64_t*) (responsePointer + 8);
            systemInfo->framebufferPitch = *(uint32_t*) (responsePointer + 16);
            systemInfo->framebufferWidth = *(uint32_t*) (responsePointer + 20);
            systemInfo->framebufferHeight = *(uint32_t*) (responsePointer + 24);
            systemInfo->framebufferBits = *(uint8_t*) (responsePointer + 25);
            systemInfo->framebufferType = *(uint8_t*) (responsePointer + 26);
            // Should assert reserved keyword is 0;
            // *(uint8_t*) (responsePointer + 27) = 0;
            break;
        default:
            BIOSPrintf("Multiboot tag of type %s (%d) not parsed.\n", tagNames[type], type);
            break;
        }

        responsePointer += size;
        responsePointer += ((uint32_t) responseAddress - (uint32_t) responsePointer) & 0x7;
    }

    return MULTIBOOT2_OK;
}


static char* tagNames[] = {
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
