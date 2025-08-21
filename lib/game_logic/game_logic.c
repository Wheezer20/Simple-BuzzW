#include <avr/io.h>
#include "game_logic.h"
#include "millis.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <display.h>
#include <stdint.h>



#define TOUCH_PIN   PD2  // Touching wire triggers fail
#define WIRE_ANALOG_PIN 5 // Analog pin A5 for wire detection
#define GOAL_PIN    PD3  // Reaching end triggers win

// Button pin definitions for ATmega328P (Velleman VMA209 shield):
#define LEFT_BUTTON_PIN   PC1 // Arduino A1
#define MIDDLE_BUTTON_PIN PC2 // Arduino A2
#define RIGHT_BUTTON_PIN  PC3 // Arduino A3

//Game State Variables:
unsigned long startTime = 0;
static unsigned long currentTime = 0;
static unsigned long lastDisplayUpdate = 0;
static bool gameRunning = false;

static bool showMessage = false;
static char currentMessage[5] = "";
static unsigned long messageStartTime = 0;

uint16_t contactCount = 0;
uint16_t* contactTimes = NULL;


game_state_t game_state = STATE_IDLE;


// Sets up the touch and goal pins as inputs with pull-up resistors
// Also sets up analog pin A5 for wire detection
void buzzwire_init(void) {

    // Set PC1, PC2, PC3 (A1, A2, A3) as inputs for buttons
    //DRC is Data Direction Register
    DDRC &= ~((1 << LEFT_BUTTON_PIN) | (1 << MIDDLE_BUTTON_PIN) | (1 << RIGHT_BUTTON_PIN));
    // Enable pull-up resistors on PC1, PC2, PC3
    PORTC |= (1 << LEFT_BUTTON_PIN) | (1 << MIDDLE_BUTTON_PIN) | (1 << RIGHT_BUTTON_PIN);


    // Analog pin A5
    DDRC &= ~(1 << DDC5);     // Set A5 as input
    PORTC |= (1 << PC5);      // Optional: Enable pull-up resistor
    printString("BuzzWire initialized complete.\r\n");
}


// Reset game state if needed (placeholder for now)
void buzzwire_start(void) {
    startTime = millis();
    currentTime = startTime;
    lastDisplayUpdate = 0;
    gameRunning = true;
    showMessage = false;
    display_clear();
}

void buzzwire_reset(void) {
    gameRunning = false;
    contactCount = 0;
    startTime = 0;
    currentTime = 0;
    lastDisplayUpdate = 0;
    showMessage = false;
    game_state = STATE_IDLE;
    // If using dynamic memory for contactTimes, free here if needed
}


// Check if the wire has been touched using analog pin A5
// Returns 1 if touched (A5 voltage low), 0 otherwise
uint8_t buzzwire_touched(void) {
    // Select analog channel 5 (A5)
    ADMUX = (ADMUX & 0xF0) | WIRE_ANALOG_PIN;
    ADCSRA |= (1 << ADSC); // Start conversion
    while (ADCSRA & (1 << ADSC)); // Wait for conversion to finish
    uint16_t value = ADC;
    // If value is low (<100), wire is touched (A5 pulled to GND)
    return (value < 100);
}


void showTempMessage(const char* msg) {
    strncpy(currentMessage, msg, 4);
    currentMessage[4] = '\0';
    showMessage = true;
    // Resets message start time
    messageStartTime = millis();
    display_write(currentMessage);
}

// Debugging function; figuring out wether wire touch is HIGH or LOW
uint8_t wireTouched(void) {
  //  return buzzwire_touched();
  if (PINC & (1 << PC5)) {
    printString("Wiretouched() = high!\r\n");
    return false;
    } else {
    printString("Wiretouched() = low.\r\n");
    return true;

}
}

uint8_t playerReachedGoal(void) {
    return !(PIND & (1 << GOAL_PIN));
}


// Returns 1 if left button (A1/PC1) is pressed (active LOW)
uint8_t leftButtonPressed(void) {
    return !(PINC & (1 << LEFT_BUTTON_PIN));
}

// Returns 1 if middle button (A2/PC2) is pressed (active LOW)
uint8_t middleButtonPressed(void) {
    return !(PINC & (1 << MIDDLE_BUTTON_PIN));
}

// Returns 1 if right button (A3/PC3) is pressed (active LOW)
uint8_t rightButtonPressed(void) {
    return !(PINC & (1 << RIGHT_BUTTON_PIN));
}