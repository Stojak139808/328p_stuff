#include <avr/io.h>
#include "include/gpio.h"

void setup_gpio(){
    DDRB = 0x00;
    DDRB |= (1 << LED_PIN);
    PORTB = 0x00;
}