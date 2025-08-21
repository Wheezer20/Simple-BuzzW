#ifndef LED_H
#define LED_H

#include <avr/io.h>
#include <util/delay.h>

#define NUMBER_OF_LEDS 4 // Corrected to 4 LEDs

void enableLed(int ledNumber);
void lightUpLed(int ledNumber);
void lightDownLed(int ledNumber);
void enableMultipleLeds(uint8_t leds);
void lightUpMultipleLeds(uint8_t leds);
void lightDownMultipleLeds(uint8_t leds);
void fadeInLed(int ledNumber, int duration);
void fadeOutLed(int ledNumber, int duration);
void indicatePlayerTurn(int playerNumber);
void showDiceSelection(uint8_t selectedDice);
void celebrateHighScore();
void dimLed(int ledNumber, int brightness, int duration);

#endif // LED_H
