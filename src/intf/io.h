#pragma once

#include "types.h"
#include "stddef.h"

void outb(u16 port, u8 v);
u8 inb(u16 port);
void delay(size_t microseconds);
