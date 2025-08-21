#include <avr/io.h>
#include <avr/interrupt.h>
#include "millis.h"

volatile unsigned long milliseconds = 0;

void millis_timer_init(void) {
    TCCR0A = (1 << WGM01); // CTC mode
    TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64
    OCR0A = 249; // 1 ms @ 16MHz
    TIMSK0 |= (1 << OCIE0A); // Enable interrupt
    sei(); // Enable global interrupts
}

ISR(TIMER0_COMPA_vect) {
    milliseconds++;
}

unsigned long millis(void) {
    unsigned long ms;
    cli();
    ms = milliseconds;
    sei();
    return ms;
}
