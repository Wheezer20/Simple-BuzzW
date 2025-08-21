#include <avr/io.h>
#include <util/delay.h>

void initADC();
uint16_t readADC();
void displayADC();

float mapPotentiometerFrequency(uint16_t value);