#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "leds.h"

void enableLed(int ledNumber) {
    if (ledNumber < 0 || ledNumber >= NUMBER_OF_LEDS) return;
    DDRB |= (1 << (PB2 + ledNumber));
}

void lightUpLed(int ledNumber) {
    if (ledNumber < 0 || ledNumber >= NUMBER_OF_LEDS) return;
    PORTB &= ~(1 << (PB2 + ledNumber));
}

void lightDownLed(int ledNumber) {
    if (ledNumber < 0 || ledNumber >= NUMBER_OF_LEDS) return;
    PORTB |= (1 << (PB2 + ledNumber));
}

void enableMultipleLeds(uint8_t leds) {
    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
        if (leds & (1 << i)) {
            enableLed(i);
        }
    }
}

void lightUpMultipleLeds(uint8_t leds) {
    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
        if (leds & (1 << i)) {
            lightUpLed(i);
        }
    }
}

void lightDownMultipleLeds(uint8_t leds) {
    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
        if (leds & (1 << i)) {
            lightDownLed(i);
        }
    }
}

void fadeInLed(int ledNumber, int duration) {
    for (int i = 1; i <= 100; i++) {
        dimLed(ledNumber, i, duration / 100.0);
    }
}

void fadeOutLed(int ledNumber, int duration) {
    for (int i = 100; i > 0; i--) {
        dimLed(ledNumber, i, duration / 100.0);
    }
}

void indicatePlayerTurn(int playerNumber) {
    if (playerNumber < 0 || playerNumber >= NUMBER_OF_LEDS) return;
    lightUpLed(playerNumber);
    _delay_ms(500);
    lightDownLed(playerNumber);
}

void showDiceSelection(uint8_t selectedDice) {
    lightUpMultipleLeds(selectedDice);
    _delay_ms(500);
    lightDownMultipleLeds(selectedDice);
}

void celebrateHighScore() {
    for (int i = 0; i < 3; i++) {
        lightUpMultipleLeds(0xFF); // Assuming all LEDs are on
        _delay_ms(200);
        lightDownMultipleLeds(0xFF);
        _delay_ms(200);
    }
}

void dimLed(int ledNumber, int brightness, int duration) {
    if (ledNumber < 0 || ledNumber >= NUMBER_OF_LEDS) return;
    for (int i = 0; i < brightness; i++) {
        lightUpLed(ledNumber);
        _delay_us(duration);
        lightDownLed(ledNumber);
        _delay_us(duration);
    }
}
