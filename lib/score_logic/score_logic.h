#ifndef SCORE_H
#define SCORE_H

#include <stdint.h>


void startGame(uint8_t mode);
void handleContact(uint8_t touched);
void stopGame(void);
void displayTimer(void);
void resetScoreLogic(void);
uint8_t gameFailedCondition(void);

#endif
