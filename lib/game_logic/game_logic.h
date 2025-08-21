#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdint.h>
#include <stdbool.h>

typedef enum { STATE_IDLE, STATE_RUNNING, STATE_FINISHED } game_state_t;

extern unsigned long startTime;
extern game_state_t game_state;

void buzzwire_init(void);
void buzzwire_start(void);
uint8_t buzzwire_touched(void);
uint8_t buzzwire_reached_goal(void);
void showTempMessage(const char* msg);
uint8_t wireTouched(void);
uint8_t playerReachedGoal(void);
void buzzwire_reset(void);
uint8_t leftButtonPressed(void);
uint8_t rightButtonPressed(void);
uint8_t middleButtonPressed(void);

#endif

