#include "display.h"
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h" 

#define SPACE 0xFF 

//defines which segments light up for each digit
const uint8_t SEGMENT_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0X80, 0X90};
//helps switch between digits (multiplexing)
const uint8_t SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};
// Map for alphabet characters A-Z and some special characters
const uint8_t ALPHABET_MAP[] = {0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xC2, 0x89, 0xCF, 0xE1, 0x8A, 0xC7, 0xEA, 0xC8, 0xC0, 0x8C, 0x4A, 0xCC, 0x92, 0x87, 0xC1, 0xC1, 0xD5, 0x89, 0x91, 0xA4, 0xFF};

// Use external display_buffer from main.c
extern char display_buffer[5];


// Initialize display pins and set known state
void display_init() {
    sbi(DDRD, LATCH_DIO);   // Set LATCH pin as output
    sbi(DDRD, CLK_DIO);     // Set CLOCK pin as output
    sbi(DDRB, DATA_DIO);    // Set DATA pin as output

    // Clear initial states
    cbi(PORTD, LATCH_DIO);
    cbi(PORTD, CLK_DIO);
    cbi(PORTB, DATA_DIO);

    printString("Display initialized\r\n");  // Debug message over Serial
}

// Safely disables display pin outputs
void disableDisplay() {
    cbi(DDRD, LATCH_DIO);
    cbi(DDRD, CLK_DIO);
    cbi(DDRB, DATA_DIO);
}

// Shifts out bits to the display shift register, acts like a clock of sorts
void shift(uint8_t val, uint8_t bitorder) {
    for (uint8_t i = 0; i < 8; i++) {
        uint8_t bit = (bitorder == LSBFIRST) ? !!(val & (1 << i)) : !!(val & (1 << (7 - i)));

        if (bit)
            sbi(PORTB, DATA_DIO);
        else
            cbi(PORTB, DATA_DIO);

        // Toggle clock to shift bit into register
        sbi(PORTD, CLK_DIO);
        cbi(PORTD, CLK_DIO);
    }
}

// Displays a number on one 7-segment digit
void writeNumberToSegment(uint8_t segment, uint8_t value) {
    cbi(PORTD, LATCH_DIO);
    shift(SEGMENT_MAP[value], MSBFIRST);          // Send digit pattern
    shift(SEGMENT_SELECT[segment], MSBFIRST);     // Activate the digit
    sbi(PORTD, LATCH_DIO);
}

// Write 4-digit number to all display segments
void writeNumber(int number) {
    if (number < 0 || number > 9999) return; // Guard range

    writeNumberToSegment(0, number / 1000);
    writeNumberToSegment(1, (number / 100) % 10);
    writeNumberToSegment(2, (number / 10) % 10);
    writeNumberToSegment(3, number % 10);
}

// Same as writeNumber() but keeps number visible for a period
void writeNumberAndWait(int number, int delay) {
    if (number < 0 || number > 9999) return;

    for (int i = 0; i < delay / 20; i++) {
        writeNumberToSegment(0, number / 1000); _delay_ms(5);
        writeNumberToSegment(1, (number / 100) % 10); _delay_ms(5);
        writeNumberToSegment(2, (number / 10) % 10); _delay_ms(5);
        writeNumberToSegment(3, number % 10); _delay_ms(5);
    }
}

// Displays a character on a single segment
void writeCharToSegment(uint8_t segment, char character) {
    int valueOfCharacter = SPACE;

    if (character >= '0' && character <= '9') {
        // Handle digits 0-9
        cbi(PORTD, LATCH_DIO);
        shift(SEGMENT_MAP[character - '0'], MSBFIRST);
        shift(SEGMENT_SELECT[segment], MSBFIRST);
        sbi(PORTD, LATCH_DIO);
    } else if (character >= 'A' && character <= 'Z') {
        valueOfCharacter = character - 'A';
        cbi(PORTD, LATCH_DIO);
        shift(ALPHABET_MAP[valueOfCharacter], MSBFIRST);
        shift(SEGMENT_SELECT[segment], MSBFIRST);
        sbi(PORTD, LATCH_DIO);
    } else if (character >= 'a' && character <= 'z') {
        valueOfCharacter = character - 'a';
        cbi(PORTD, LATCH_DIO);
        shift(ALPHABET_MAP[valueOfCharacter], MSBFIRST);
        shift(SEGMENT_SELECT[segment], MSBFIRST);
        sbi(PORTD, LATCH_DIO);
    } else {
        // Fallback: blank/invalid char
        cbi(PORTD, LATCH_DIO);
        shift(0xFF, MSBFIRST);  // All segments off
        shift(SEGMENT_SELECT[segment], MSBFIRST);
        sbi(PORTD, LATCH_DIO);
    }
}

// Display 4 characters for specified time
void display_writeStringAndWait(char *str, int delay) {
    display_init();  // Ensure display is active

    for (int i = 0; i < delay; i++) {
        writeCharToSegment(0, str[0]); _delay_ms(5);
        writeCharToSegment(1, str[1]); _delay_ms(5);
        writeCharToSegment(2, str[2]); _delay_ms(5);
        writeCharToSegment(3, str[3]); _delay_ms(5);
    }
}

// Clears the 4-digit display
void display_clear() {
    for (int i = 0; i < 4; i++) {
        writeCharToSegment(i, ' ');
    }
}

// Disables a single digit (makes it blank)
void blankSegment(uint8_t segment) {
    cbi(PORTD, LATCH_DIO);
    shift(0xFF, MSBFIRST);  // 0xFF = all segments off
    shift(SEGMENT_SELECT[segment], MSBFIRST);
    sbi(PORTD, LATCH_DIO);
}

// Displays a fixed string instantly
void display_write(const char *str) {
    for (int i = 0; i < 4; i++) {
        writeCharToSegment(i, str[i]);
    }
}

// Refreshes display with one character at a time (for timer interrupt mode)
void display_refresh(const char *text) {
    static uint8_t digit = 0;
    writeCharToSegment(digit, text[digit]);
    digit = (digit + 1) % 4;
}

// Copy string to buffer for later display
void display_writeString(const char* str) {
    for (int i = 0; i < 4; i++) {
        display_buffer[i] = (str[i] == '\0') ? ' ' : str[i];
    }
    display_buffer[4] = '\0';  // Null terminate
}

// Formats and displays MM:SS time format
void display_showTime(uint32_t seconds) {
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02lu:%02lu", seconds / 60, seconds % 60);
    display_refresh(buffer);
}

void display_tick(void) {
    static uint8_t digit = 0;
    writeCharToSegment(digit, display_buffer[digit]);  // Display one digit at a time
    digit = (digit + 1) % 4;  // Cycle through all 4 digits
}