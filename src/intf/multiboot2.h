#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "bios.h"

#define MULTIBOOT2_BOOTLOADER_MAGIC         0x36d76289

#define MULTIBOOT2_OK                       0
#define MULTIBOOT2_INVALID_MAGIC            1
#define MULTIBOOT2_INVALID_RESERVED_FIELD   2

#define MULTIBOOT2_TAG_CMD_LINE             1
#define MULTIBOOT2_TAG_BOOTLOADER_NAME      2
#define MULTIBOOT2_TAG_BASIC_MEMORY         4
#define MULTIBOOT2_TAG_BOOT_DEVICE          5
#define MULTIBOOT2_TAG_MEMORY_MAP           6
#define MULTIBOOT2_TAG_FRAMEBUFFER          8


typedef struct MultibootInfo {
    bool     valid;
    uint32_t responseAddress;
    uint32_t totalHeaderSize;

    // Currently a pointer to the memory returned by the
    // bootloader. Should copy this once the heap is set up.
    char*    commandLineString;
    
    size_t   lowerMemoryAmount;
    size_t   upperMemoryAmount;

    uint32_t bootDeviceNumber;
    uint32_t bootDevicePartition;
    uint32_t bootDeviceSubpartition;

    uint64_t framebufferAddress;
    uint32_t framebufferPitch;
    uint32_t framebufferWidth;
    uint32_t framebufferHeight;
    uint8_t  framebufferBits;
    uint8_t  framebufferType;
} MultibootInfo;

// Parses Multiboot response into systemInfo struct and returns true if successful
void Multiboot2GetSystemInfo(uint32_t magic, void* responseAddress, MultibootInfo* systemInfo);
void Multiboot2PrintResponse(MultibootInfo* systemInfo);