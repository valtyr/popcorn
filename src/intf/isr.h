#pragma once

#include <stddef.h>
#include <bios.h>

void ISRRegisterHandler(uint8_t n, ISRHandler handler);