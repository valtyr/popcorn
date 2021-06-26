#include "pit.h"

static ISRHandler s_pit_handler = NULL;

void __pit_handler(registers_t reg)
{
    if (s_pit_handler != NULL)
        s_pit_handler(reg);
}

void __pit_cmd_tx(u8 cmd)
{
    outb(PIT_REG_COMMAND, cmd);
}

void __pit_data_tx(u8 data, u8 counter)
{
    u8 port = (counter == PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER0 : ((counter == PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER1 : PIT_REG_COUNTER2);
    outb(port, data);
}

void __pit_start_counter(u32 freq, u8 counter, u8 mode)
{
    if (freq == 0)
        return;

    BIOSPrintf("PIT counter %d started with frequency %dHz\n", counter / 0x40, freq);

    uint16_t divisor = (uint16_t)(1193181 / (uint16_t)freq);

    // send operational command words
    uint8_t ocw = 0;
    ocw = (ocw & ~PIT_OCW_MASK_MODE) | mode;
    ocw = (ocw & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_DATA;
    ocw = (ocw & ~PIT_OCW_MASK_COUNTER) | counter;
    __pit_cmd_tx(ocw);

    // set frequency rate
    __pit_data_tx(divisor & 0xff, 0);
    __pit_data_tx((divisor >> 8) & 0xff, 0);
}

void PITInit()
{
    BIOSPrintf("Initializing PIT...\n");

    ISRRegisterHandler(IRQ_0, __pit_handler);

    __pit_start_counter(60, PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);

    BIOSPrintf("PIT initialized\n");
}

void PITAttach(ISRHandler handler)
{
    if (s_pit_handler != NULL)
    {
        BIOSPrint("Warning: PIT handler overwritten\n");
    }
    s_pit_handler = handler;
}

void PITDetach()
{
    s_pit_handler = NULL;
}
