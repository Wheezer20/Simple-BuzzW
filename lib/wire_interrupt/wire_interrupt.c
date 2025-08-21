#include <avr/io.h>
#include <avr/interrupt.h>
#include "wire_interrupt.h"
#include "usart.h"

// Flag set by interrupt when wire is touched
volatile uint8_t wire_touched_flag = 0;
volatile uint8_t last_wire_state = 1; // PC5 starts HIGH (not touched)

void wire_interrupt_init(void) {
    // Configure PC5 as input with pull-up (same setup as wireTouched())
    DDRC &= ~(1 << 5);      // Set PC5 as input
    PORTC |= (1 << 5);      // Enable pull-up resistor
    
    // Enable Pin Change Interrupt for PORTC
    PCICR |= (1 << 1);      // Enable PCIE1)
    
    // Enable interrupt specifically for PC5
    PCMSK1 |= (1 << 5);     // Enable PCINT13 (PC5)
    
    // Initialize last state
    last_wire_state = (PINC & (1 << 5)) ? 1 : 0;
    
    // Enable da global interrupts
    sei();
}

// Interrupt Service Routine
ISR(PCINT1_vect) {
    // Read current state of PC5 (wire touch/detection pin)
    uint8_t current_state = (PINC & (1 << 5)) ? 1 : 0;
    
    // Check if PC5 changed from HIGH to LOW (wire touched)
    if (last_wire_state == 1 && current_state == 0) {
        wire_touched_flag = 1;  // Set flag for main program
    }
    
    // Update last state
    last_wire_state = current_state;
}

uint8_t wire_touched_interrupt(void) {
    return wire_touched_flag;
}

void wire_touch_reset(void) {
    wire_touched_flag = 0;
}
