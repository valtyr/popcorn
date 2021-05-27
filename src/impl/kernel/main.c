#include "bios.h"


static void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }

void kernel_main() {
    BIOSClear();
    BIOSSetColor(BIOS_COLOR_BLACK, BIOS_COLOR_YELLOW);
    BIOSPrint(">>> Popcorn kernel 64! ");
    BIOSPrintChar(2);
    BIOSPrint(" <<<\n");


    BIOSSetColor(BIOS_COLOR_WHITE, BIOS_COLOR_BLACK);
    BIOSPrint("Test\n");

    while(true) {
        BIOSWaitArbitrary();
        BIOSPrint("Test\n");
    }



    play_sound(440);

    BIOSHalt();
}
