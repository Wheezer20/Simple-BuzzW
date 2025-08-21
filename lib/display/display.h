#ifndef DISPLAY_H
#define DISPLAY_H

#include <avr/io.h>
#include <util/delay.h>
#include "usart.h" 

// Segment control
#define LOW 0
#define HIGH 1

// Pin configuration
#define LATCH_DIO PD4
#define CLK_DIO   PD7
#define DATA_DIO  PB0

// Constants
#define SPACE 0xFF
#define DOT   0x2E
#define LSBFIRST 0
#define MSBFIRST 1
#define NUMBER_OF_SEGMENTS 8

// Bit macros
#define sbi(register, bit) ((register) |= _BV(bit))
#define cbi(register, bit) ((register) &= ~_BV(bit))

// Function declarations
void display_clear(void);
void display_tick(void);
void disableDisplay(void);
void shift(uint8_t val, uint8_t bitorder);
void writeNumberToSegment(uint8_t segment, uint8_t value);
void writeNumber(int number);
void writeNumberAndWait(int number, int delay);
void scrollingNumbers(int a, int b, int c, int d);
void writeCharToSegment(uint8_t segment, char character);
void display_writeStringAndWait(char *str, int delay);
void display_init(void);
void display_writeString(const char *str);
void display_write(const char *str);
void display_refresh(const char *text);
void display_showTime(uint32_t seconds);


/*
had to change the display_refresh/string writing code from my last game because that shi was not stable
multiplexing fixed the issue fn i guess?

#include "display.h"  // Adjust path if needed
#include <util/delay.h>

int main(void) {
    display_init();  // Assuming you have this to initialize your display

    const char *text = "TEST";

    while (1) {
        display_refresh(text);  // Call with argument
        _delay_ms(5);           // Delay for visible multiplexing
    }

    return 0;
}

USED ABOVE FOR TESTING
*/



#endif // DISPLAY_H

