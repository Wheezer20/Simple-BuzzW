#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Timer1 initialization and control
void timer1_init(void);
void timer1_start(uint16_t ocr, void (*callback)(void));
void timer1_stop(void);

void setup_game_timer(void (*callback)(void));  // Setup timer1 for the game tick


// Time getters
uint32_t millis(void);
uint32_t getSeconds(void);

#endif // TIMER_H
