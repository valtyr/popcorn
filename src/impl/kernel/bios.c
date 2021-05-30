#include "bios.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;


struct Char {
    uint8_t character;
    uint8_t color;
};

// Pointer to first char in buffer
struct Char* buffer = (struct Char*) 0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = BIOS_COLOR_WHITE | BIOS_COLOR_BLACK << 4;


// Utility functions

void clear_row(size_t row) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };

    for (size_t col = 0; col < NUM_COLS; col++) {
        buffer[col + NUM_COLS * row] = empty;
    }
}

void print_newline() {
    col = 0;
    if(row < NUM_ROWS - 1) {
        row++;
        return;
    }

    // Scroll buffer up
    for (size_t row = 1; row < NUM_ROWS; row++) {
        for (size_t col = 0; col < NUM_COLS; col++) {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row - 1)] = character;
        }
    }
    
    clear_row(NUM_ROWS - 1);
}

static void itoa (char *buf, int base, int d) {
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;
  
  /* If %d is specified and D is minus, put `-' in the head.  */
  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;

  /* Divide UD by DIVISOR until UD == 0.  */
  do
    {
      int remainder = ud % divisor;
      
      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);

  /* Terminate BUF.  */
  *p = 0;
  
  /* Reverse BUF.  */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}



// Meat

void BIOSClear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        clear_row(i);
    }
}

void BIOSPrintChar(char character) {
    if (character == '\n') {
        print_newline();
        outb((uint8_t) '\r', 0x3f8);
        outb((uint8_t) '\n', 0x3f8);
        return;
    }

    if (col > NUM_COLS) {
        print_newline();
    }


    outb((uint8_t) character, 0x3f8);

    buffer[col + NUM_COLS * row] = (struct Char) {
        character: (uint8_t) character,
        color: color,
    };

    col++;
}

void BIOSPrint(char* string) {
    for (size_t i = 0; 1; i++) {
        char character = (uint8_t) string[i];

        if(character == '\0') {
            return;
        }

        BIOSPrintChar(character);
    }
}

void BIOSPrintBlink(char* string) {
    for (size_t i = 0; 1; i++) {
        char character = (uint8_t) string[i];

        if(character == '\0') {
            return;
        }

        BIOSPrintChar(character);
    }
}

void BIOSPrintFancy(char* string) {
    for (size_t i = 0; 1; i++) {
        char character = (uint8_t) string[i];

        if(character == '\0')
            return;

        BIOSPrintChar(character);
    }
}

void BIOSSetColor(BIOSColor foreground, BIOSColor background) {
    color = foreground | background << 4;
}

void BIOSPanic(char* message) {
    BIOSSetColor(BIOS_COLOR_WHITE, BIOS_COLOR_RED);
    print_newline();
    BIOSPrint("Panic! ");
    BIOSPrint(message);
    BIOSPrint(" at: ");
    BIOSPrint(__FILE__);

    BIOSHalt();
}


void BIOSPrintf (const char *format, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[20];

  arg++;
  
  while ((c = *format++) != 0)
    {
      if (c != '%')
	BIOSPrintChar (c);
      else
	{
	  char *p, *p2;
	  int pad0 = 0, pad = 0;
	  
	  c = *format++;
	  if (c == '0')
	    {
	      pad0 = 1;
	      c = *format++;
	    }

	  if (c >= '0' && c <= '9')
	    {
	      pad = c - '0';
	      c = *format++;
	    }

	  switch (c)
	    {
	    case 'd':
	    case 'u':
	    case 'x':
	      itoa (buf, c, *((int *) arg++));
	      p = buf;
	      goto string;
	      break;

	    case 's':
	      p = *arg++;
	      if (! p)
		p = "(null)";

	    string:
	      for (p2 = p; *p2; p2++);
	      for (; p2 < p + pad; p2++)
		BIOSPrintChar (pad0 ? '0' : ' ');
	      while (*p)
		BIOSPrintChar (*p++);
	      break;

	    default:
	      BIOSPrintChar (*((int *) arg++));
	      break;
	    }
	}
    }
}

void BIOSHexdump(uint8_t* address, size_t length) {
    size_t offset = 0;

    char printables[16];


    BIOSPrintf(" ---");
    while(offset < length) {
        // Offset mod 16 == 0

        int col = offset & 0xF; 

        if (col == 0) {
            if(offset != 0)
                BIOSPrintf("  %s", printables);
            BIOSPrintf("\n 0x%08x: ", address + offset);
        }

        if(address[offset] >= 0x20 && address[offset] <= 0x7e) {
            printables[col] = address[offset];
        } else
            printables[col] = '.';

        BIOSPrintf(
            "%x%x ",
            address[offset] >> 4,
            address[offset] & 0xF
        );       

        offset++;
    }

    if(length & 0xF != 0) {
        for(int i = offset & 0xF; i < 16; i++) {
            BIOSPrintf("XX ");
            printables[i] = ' ';
        }
    }
    printables[16] = '\0';
    BIOSPrintf("  %s\n ---\n", printables);
}
