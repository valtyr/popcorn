#include "dt.h"

GDTEntry gdt_entries[GDT_ENTRIES];
GDTPointer gdt_ptr;

IDTEntry idt_entries[IDT_ENTRIES];
IDTPointer idt_ptr;

static void GDTSetGate(u32 num, u32 base, u32 limit, u8 access, u8 gran)
{
    // Setup the descriptor base address
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    // Setup the descriptor limits
    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = ((limit >> 16) & 0x0F);

    // Finally, set up the granularity and access flags
    gdt_entries[num].granularity |= (gran & 0xF0);
    gdt_entries[num].access = access;
}

static void InitGDT()
{
    gdt_ptr.limit = (sizeof(GDTEntry) * GDT_ENTRIES) - 1;
    gdt_ptr.base = (u32)&gdt_entries;

    GDTSetGate(0, 0, 0, 0, 0);                              // Null segment
    GDTSetGate(1, SEGMENT_BASE, SEGMENT_LIMIT, 0x9A, 0xCF); // Kernel Code segment
    GDTSetGate(2, SEGMENT_BASE, SEGMENT_LIMIT, 0x92, 0xCF); // Kernel Data segment
    GDTSetGate(3, SEGMENT_BASE, SEGMENT_LIMIT, 0xFA, 0xCF); // User mode code segment
    GDTSetGate(4, SEGMENT_BASE, SEGMENT_LIMIT, 0xF2, 0xCF); // User mode data segment

    // Was cast to u32?
    GDTFlush(&gdt_ptr);
}

static void IDTSetGate(u8 num, u32 base, u16 sel, u8 flags)
{
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt_entries[num].flags = flags /* | 0x60 */;
}

static void InitIDT()
{
    idt_ptr.limit = sizeof(IDTEntry) * 256 - 1;
    idt_ptr.base = (u32)&idt_entries;

    // Remap the irq table.
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    // Setup ISRs
    IDTSetGate(0, (u32)isr0, 0x08, 0x8E);   // Division by zero
    IDTSetGate(1, (u32)isr1, 0x08, 0x8E);   // Single-step interrupt
    IDTSetGate(2, (u32)isr2, 0x08, 0x8E);   // NMI
    IDTSetGate(3, (u32)isr3, 0x08, 0x8E);   // Breakpoint
    IDTSetGate(4, (u32)isr4, 0x08, 0x8E);   // Overflow
    IDTSetGate(5, (u32)isr5, 0x08, 0x8E);   // Bound range exceeded
    IDTSetGate(6, (u32)isr6, 0x08, 0x8E);   // Invalid opcode
    IDTSetGate(7, (u32)isr7, 0x08, 0x8E);   // Coprocessor not available
    IDTSetGate(8, (u32)isr8, 0x08, 0x8E);   // Double fault
    IDTSetGate(9, (u32)isr9, 0x08, 0x8E);   // Coprocessor segment overload
    IDTSetGate(10, (u32)isr10, 0x08, 0x8E); // Invalid task state segment
    IDTSetGate(11, (u32)isr11, 0x08, 0x8E); // Segment not present
    IDTSetGate(12, (u32)isr12, 0x08, 0x8E); // Stack segment fault
    IDTSetGate(13, (u32)isr13, 0x08, 0x8E); // General protection fault
    IDTSetGate(14, (u32)isr14, 0x08, 0x8E); // Page fault
    IDTSetGate(15, (u32)isr15, 0x08, 0x8E); // RESERVED
    IDTSetGate(16, (u32)isr16, 0x08, 0x8E); // x87 floating point exception
    IDTSetGate(17, (u32)isr17, 0x08, 0x8E); // Alignment check
    IDTSetGate(18, (u32)isr18, 0x08, 0x8E); // Machine check
    IDTSetGate(19, (u32)isr19, 0x08, 0x8E); // SIMD floating point exception
    IDTSetGate(20, (u32)isr20, 0x08, 0x8E); // Virtulization exception
    IDTSetGate(21, (u32)isr21, 0x08, 0x8E); // Control protection exception
    IDTSetGate(22, (u32)isr22, 0x08, 0x8E);
    IDTSetGate(23, (u32)isr23, 0x08, 0x8E);
    IDTSetGate(24, (u32)isr24, 0x08, 0x8E);
    IDTSetGate(25, (u32)isr25, 0x08, 0x8E);
    IDTSetGate(26, (u32)isr26, 0x08, 0x8E);
    IDTSetGate(27, (u32)isr27, 0x08, 0x8E);
    IDTSetGate(28, (u32)isr28, 0x08, 0x8E);
    IDTSetGate(29, (u32)isr29, 0x08, 0x8E);
    IDTSetGate(30, (u32)isr30, 0x08, 0x8E);
    IDTSetGate(31, (u32)isr31, 0x08, 0x8E);

    // Setup IRQs
    IDTSetGate(32, (u32)irq0, 0x08, 0x8E);  // System timer
    IDTSetGate(33, (u32)irq1, 0x08, 0x8E);  // PS2 Keyboard
    IDTSetGate(34, (u32)irq2, 0x08, 0x8E);  // Cascaded signals from 8-15
    IDTSetGate(35, (u32)irq3, 0x08, 0x8E);  // Serial port controller SP2
    IDTSetGate(36, (u32)irq4, 0x08, 0x8E);  // Serial port controller SP1
    IDTSetGate(37, (u32)irq5, 0x08, 0x8E);  // Parallel port 2 and 3 or sound card
    IDTSetGate(38, (u32)irq6, 0x08, 0x8E);  // Floppy disk controller
    IDTSetGate(39, (u32)irq7, 0x08, 0x8E);  // Parallel port 1
    IDTSetGate(40, (u32)irq8, 0x08, 0x8E);  // RTC
    IDTSetGate(41, (u32)irq9, 0x08, 0x8E);  // ACPI
    IDTSetGate(42, (u32)irq10, 0x08, 0x8E); // Open for peripherals
    IDTSetGate(43, (u32)irq11, 0x08, 0x8E); // Open for peripherals
    IDTSetGate(44, (u32)irq12, 0x08, 0x8E); // PS2 Mouse
    IDTSetGate(45, (u32)irq13, 0x08, 0x8E); // Co-processor or FPU or IPI
    IDTSetGate(46, (u32)irq14, 0x08, 0x8E); // Primary ATA channel
    IDTSetGate(47, (u32)irq15, 0x08, 0x8E); // Secondary ATA channel

    // Load IDT
    IDTFlush(&idt_ptr);
}

void InitDescriptorTables()
{
    // Initialise the global descriptor table
    InitGDT();

    // Initialise the interrupt descriptor table
    InitIDT();
}