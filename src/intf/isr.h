#pragma once

#include <stddef.h>
#include <bios.h>

typedef void (*ISRHandler)(registers_t);
void ISRRegisterHandler(uint8_t n, ISRHandler handler);