#ifndef WIRE_INTERRUPT_H
#define WIRE_INTERRUPT_H

#include <stdint.h>

// Initialize wire touch interrupt on PC5 (PCINT13)
// Uses same pin as existing wireTouched() function for compatibility
void wire_interrupt_init(void);

// Check if wire was touched (interrupt-driven)
uint8_t wire_touched_interrupt(void);

// Reset the wire touch flag
void wire_touch_reset(void);

#endif
