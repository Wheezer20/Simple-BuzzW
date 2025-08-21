#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <potentiometer.h>

void initADC()
{
    ADMUX |= (1 << REFS0); // Set up the reference voltage. We choose 5V as the reference.
    //ADMUX &= ~(1 << MUX3) & ~(1 << MUX2) & ~(1 << MUX1) & ~(1 << MUX0);
    // Set MUX0-3 to zero to read analog input from PC0
    // Default is 0000 so this setting is not really necessary
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Determine the sample rate by setting the division factor to 128.
    ADCSRA |= (1 << ADEN);                                // Enable the ADC

    ADCSRA |= (1 << ADATE);                               // Enable ADC Auto Triggering
    ADCSRB = 0;                                           // Set ADC Auto Trigger Source to Free Running Mode (default)
    ADCSRA |= (1 << ADSC);                                
}

uint16_t readADC()
{

    //ADCSRA |= (1 << ADSC); // Start the analog-to-digital conversion
    //loop_until_bit_is_clear(ADCSRA, ADSC);
   // uint16_t value = ADC;
    _delay_ms(100);
    //return value;
    return ADC;
}

void displayADC()
{
    while (1)
    {
        uint16_t value = readADC();

        int displayVal = (int)value;
        writeNumberAndWait(value, 100);
    }
}

float mapPotentiometerFrequency(uint16_t value){

    float frequency = (float) value * (1000.0 - 100) / 1023.0 + 100;
    return frequency;

}