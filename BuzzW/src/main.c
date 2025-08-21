#include "leds.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "score_logic.h"
#include "game_logic.h"
#include "display.h"
#include "buzzer.h"
#include "millis.h"
#include "button.h"
#include "wire_interrupt.h"



char display_buffer[5] = "0000";  // Global display buffer



int main(void) {
    initUSART();
    enableLed(0);
    lightDownLed(0); //making sure LED is off at the start 
    printString("Main loop started\r\n");
    // Init hardware and libs
    display_init();
    buzzer_init();       // Initialize buzzer
    disableBuzzer();     // Ensure buzzer is OFF at startup
    millis_timer_init();
    wire_interrupt_init(); // Initialize wire touch interrupt
    buzzwire_init();
    initButton();
    printString("Buttons initialized\r\n");

    // Main loop
    static game_state_t last_printed_state = -1; //tracks last printed state, stops state spam in serial monitor
    while (1) {
        display_tick();
        _delay_ms(2);

        switch (game_state) {
            case STATE_IDLE:
                snprintf(display_buffer, 5, "RDY ");
                if (last_printed_state != STATE_IDLE) {
                    printString("State is: IDLE, please press the left button to start the game...\r\n");
                    last_printed_state = STATE_IDLE;
                }
          

                if (leftButtonPressed()) {
                    printString("Left button pressed. Starting game.\r\n");
                    startGame(0);
                    buzzwire_start();
                    game_state = STATE_RUNNING;
                    // Let displayTimer() handle the display buffer
                }
                break;

            case STATE_RUNNING: {
                if (last_printed_state != STATE_RUNNING) {
                    printString("State: RUNNING\r\n");
                    last_printed_state = STATE_RUNNING;
                }
                static uint8_t lastTouch = 0;

                // Check for wire contact using interrupt-driven detection
                uint8_t touched = wire_touched_interrupt();

                static unsigned long led_on_time = 0;
                // If wire is touched and it wasn't touched last time, handle the contact
                if (touched && !lastTouch) {
                    printString("Wire touched in main!\r\n");
                    handleContact(touched);
                    // Turn on LED 0 and record the time
                    lightUpLed(0);
                    led_on_time = millis();
                    // Play short buzz when wire is touched (A5 note for 150ms)
                    playTone(880.0, 150);  // A5 note for 150ms - quick buzz
                    // Reset the interrupt flag after handling
                    wire_touch_reset();
                }
                // Turn off LED 0 after 1 second
                if ((millis() - led_on_time >= 1000)) {
                    lightDownLed(0);
                }
                lastTouch = touched;

                displayTimer();

                if (rightButtonPressed()) {
                    printString("Right button pressed. Stopping game.\r\n");
                    game_state = STATE_FINISHED;
                }
                break;
            }

            case STATE_FINISHED:
                if (last_printed_state != STATE_FINISHED) {
                    printString("State: FINISHED\r\n");
                    last_printed_state = STATE_FINISHED;
                }
                snprintf(display_buffer, 5, "DONE");
                stopGame(); // Statistics printing
                _delay_ms(5000);
                printString("Game has ended, Thanks for playing!.\r\n");
                printString("\n Resetting game state.\r\n");
                resetScoreLogic();
                buzzwire_reset();
                game_state = STATE_IDLE;
                break;
        }
    }
    return 0;
}