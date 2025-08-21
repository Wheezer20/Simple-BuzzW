
#ifndef BUTTONLIB_H
#define BUTTONLIB_H

#include <avr/io.h>

#define BUTTON_START 0  // PC1
#define BUTTON_RESET 1  // PC2

#define DEBOUNCE_DELAY 200  // ms
#define NUMBER_OF_BUTTONS 3

// Function declarations
void enableButton(int button);
int buttonPushed(int button);
int buttonReleased(int button);
void initButton(void);

#endif
