#include "io.h"

void outb(u16 port, u8 v)
{
	__asm__ volatile("outb %0,%1"
					 :
					 : "a"(v), "dN"(port));
}

u8 inb(u16 port)
{
	u8 v;
	__asm__ volatile("inb %1,%0"
					 : "=a"(v)
					 : "dN"(port));
	return v;
}
