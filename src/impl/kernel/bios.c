#include "bios.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 80;


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