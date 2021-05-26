#pragma once

#include <stdint.h>

void outb(uint8_t v, uint16_t port);
uint8_t inb(uint16_t port);