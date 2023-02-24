#ifndef GPIO_H
#define GPIO_H

#include "mcu.h"
#include <avr/io.h>

#define LED_PIN DD5

#define TOGGLE_LED_PIN() PORTB ^= 1u << LED_PIN

void setup_gpio(void);

#endif
