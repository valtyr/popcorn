#pragma once

#include <stddef.h>
#include "types.h"
#include "io.h"

#define SEGMENT_BASE 0           // Use Flat addressing
#define SEGMENT_LIMIT 0xFFFFFFFF // Allow it to access all of 4GB memory

#define GDT_ENTRIES 5
#define IDT_ENTRIES 256

typedef struct GDTEntry
{
    u16 limit_low;  /* The lower 16 bits of the limit */
    u16 base_low;   /* The lower 16 bits of the base */
    u8 base_middle; /* The next 8 bits of the base */
    u8 access;      /* Contains access flags */
    u8 granularity; /* Specify granularity, and 4 bits of limit */
    u8 base_high;   /* The last 8 bits of the base; */
} __attribute__((packed)) GDTEntry;
/* It needs to be packed like this, 64 bits */

typedef struct GDTPointer
{
    u16 limit; /* Size of gdt table in bytes*/
    u32 base;  /* Address to the first gdt entry */
} __attribute__((packed)) GDTPointer;

extern void GDTFlush(GDTPointer *);

// A struct describing an interrupt gate
typedef struct IDTEntry
{
    u16 base_lo; // The lower 16 bits of the address to jump to when this interrupt fires.
    u16 sel;     // Kernel segment selector.
    u8 always0;  // This must always be zero.
    u8 flags;    // More flags. See documentation.
    u16 base_hi; // The upper 16 bits of the address to jump to.
} __attribute__((packed)) IDTEntry;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
typedef struct IDTPointer
{
    u16 limit;
    u32 base; // The address of the first element in our idt_entry_t array.
} __attribute__((packed)) IDTPointer;

extern void IDTFlush(IDTPointer *);

static void InitGDT();
static void GDTSetGate(u32 num, u32 base, u32 limit, u8 access, u8 gran);
static void InitIDT();
void InitDescriptorTables();

// Default empty ISRs
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// Default empty IRQs
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();