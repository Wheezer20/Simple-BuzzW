#include <avr/io.h>
#include <avr/interrupt.h>
#include "button.h"
#include "millis.h"
#include <stdio.h>

int buttonPushed(int button) {
    if (button < 0 || button >= NUMBER_OF_BUTTONS) return 0;
    // Read pin directly (active LOW) since interrupt is disabled
    return !(PINC & (1 << (PC1 + button)));
}

int buttonReleased(int button) {
    if (button < 0 || button >= NUMBER_OF_BUTTONS) return 1;
    // Read pin directly (active LOW) since interrupt is disabled  
    return (PINC & (1 << (PC1 + button))) ? 1 : 0;
}

void enableButton(int button) {
    if (button >= 0 && button < NUMBER_OF_BUTTONS) {
        DDRC &= ~(1 << (PC1 + button));  //creates a bitmask for pin, Set as input
        PORTC |= (1 << (PC1 + button));  // Enable pull-up
    }
}

void initButton(void) {
    for (int i = 0; i < NUMBER_OF_BUTTONS; i++) {
        enableButton(i);
    }
}