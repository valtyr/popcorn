#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "bios.h"
#include "types.h"

#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289

#define MULTIBOOT2_OK 0
#define MULTIBOOT2_INVALID_MAGIC 1
#define MULTIBOOT2_INVALID_RESERVED_FIELD 2

#define MULTIBOOT2_TAG_CMD_LINE 1
#define MULTIBOOT2_TAG_BOOTLOADER_NAME 2
#define MULTIBOOT2_TAG_BASIC_MEMORY 4
#define MULTIBOOT2_TAG_BOOT_DEVICE 5
#define MULTIBOOT2_TAG_MEMORY_MAP 6
#define MULTIBOOT2_TAG_FRAMEBUFFER 8
#define MULTIBOOT2_TAG_ELF_SYMBOLS 9

typedef struct MultibootInfo
{
    bool valid;
    u32 responseAddress;
    u32 totalHeaderSize;

    // Currently a pointer to the memory returned by the
    // bootloader. Should copy this once the heap is set up.
    char *commandLineString;

    size_t lowerMemoryAmount;
    size_t upperMemoryAmount;

    u32 bootDeviceNumber;
    u32 bootDevicePartition;
    u32 bootDeviceSubpartition;

    u64 framebufferAddress;
    u32 framebufferPitch;
    u32 framebufferWidth;
    u32 framebufferHeight;
    u8 framebufferBits;
    u8 framebufferType;

    u16 elfSymbolNumber;
    u16 elfEntsize;
    u16 elfShndx;
} MultibootInfo;

// Parses Multiboot response into systemInfo struct and returns true if successful
void Multiboot2GetSystemInfo(u32 magic, void *responseAddress, MultibootInfo *systemInfo);
void Multiboot2PrintResponse(MultibootInfo *systemInfo);