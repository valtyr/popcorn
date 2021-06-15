#pragma once

#include <stddef.h>
#include <bios.h>

typedef enum
{
    /* Division by zero */
    ISR_0 = 0,
    /* Single-step interrupt */
    ISR_1 = 1,
    /* NMI */
    ISR_2 = 2,
    /* Breakpoint */
    ISR_3 = 3,
    /* Overflow */
    ISR_4 = 4,
    /* Bound range exceeded */
    ISR_5 = 5,
    /* Invalid opcode */
    ISR_6 = 6,
    /* Coprocessor not available */
    ISR_7 = 7,
    /* Double fault */
    ISR_8 = 8,
    /* Coprocessor segment overload */
    ISR_9 = 9,
    /* Invalid task state segment */
    ISR_10 = 10,
    /* Segment not present */
    ISR_11 = 11,
    /* Stack segment fault */
    ISR_12 = 12,
    /* General protection fault */
    ISR_13 = 13,
    /* Page fault */
    ISR_14 = 14,
    /* RESERVED */
    ISR_15 = 15,
    /* x87 floating point exception */
    ISR_16 = 16,
    /* Alignment check */
    ISR_17 = 17,
    /* Machine check */
    ISR_18 = 18,
    /* SIMD floating point exception */
    ISR_19 = 19,
    /* Virtulization exception */
    ISR_20 = 20,
    /* Control protection exception */
    ISR_21 = 21,
    ISR_22 = 22,
    ISR_23 = 23,
    ISR_24 = 24,
    ISR_25 = 25,
    ISR_26 = 26,
    ISR_27 = 27,
    ISR_28 = 28,
    ISR_29 = 29,
    ISR_30 = 30,
    ISR_31 = 31,

    /* System timer */
    IRQ_0 = 32,
    /* PS2 keyboard */
    IRQ_1 = 33,
    /* Cascaded signals from IRQ 8-15 */
    IRQ_2 = 34,
    /* Serial port controller 2 */
    IRQ_3 = 35,
    /* Serial port controller 1 */
    IRQ_4 = 36,
    /* Parallel port 2 and 3 or sound card */
    IRQ_5 = 37,
    /* Floppy disk controller */
    IRQ_6 = 38,
    /* Parallel port 1 */
    IRQ_7 = 39,
    /* RTC */
    IRQ_8 = 40,
    /* ACPI */
    IRQ_9 = 41,
    /* Open for peripherals */
    IRQ_10 = 42,
    /* Open for peripherals */
    IRQ_11 = 43,
    /* PS2 mouse */
    IRQ_12 = 44,
    /* Co-processor or FPU */
    IRQ_13 = 45,
    /* Primary ATA channel */
    IRQ_14 = 46,
    /* Secondary ATA channel */
    IRQ_15 = 47,
} ISRIdentifier;

typedef void (*ISRHandler)(registers_t);
void ISRRegisterHandler(ISRIdentifier n, ISRHandler handler);
