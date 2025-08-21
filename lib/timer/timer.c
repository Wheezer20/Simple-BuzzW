#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint32_t timerCount = 0; // Timer count in seconds

void initTimer1(void) {
    // Set Timer1 in CTC (Clear Timer on Compare Match) mode
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC mode with prescaler 1024

    // Set compare value for 1 second interval (assuming 16MHz clock and prescaler 1024)
    OCR1A = 15624;  // 16000000 / (1024 * 1) - 1 = 15624

    // Enable Timer1 compare match interrupt
    TIMSK1 = (1 << OCIE1A);

    // Clear the timer register
    TCNT1 = 0;

    // Enable global interrupts
    sei();
}

ISR(TIMER1_COMPA_vect) {
    timerCount++; // Increment count every second
}

void startTimer(void) {
    timerCount = 0; // Reset timer count
}

uint32_t getElapsedTime(void) {
    return timerCount; // Return elapsed time in seconds
}
