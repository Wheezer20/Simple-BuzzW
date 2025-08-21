#ifndef BUZZER_H
#define BUZZER_H

#include <stdint.h>

void buzzer_init(void);
void enableBuzzer(void);
void disableBuzzer(void);
void playTone(float frequency, uint32_t duration_ms);  // duration in ms

#endif

