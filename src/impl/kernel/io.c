#include "io.h"

void outb(uint16_t port, uint8_t v)
{
	__asm__ volatile("outb %0,%1"
					 :
					 : "a"(v), "dN"(port));
}

uint8_t inb(uint16_t port)
{
	uint8_t v;
	__asm__ volatile("inb %1,%0"
					 : "=a"(v)
					 : "dN"(port));
	return v;
}
