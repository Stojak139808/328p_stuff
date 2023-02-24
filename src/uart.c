#include "include/uart.h"
#include "include/mcu.h"
#include "include/gpio.h"
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(USART_RX_vect){
    uint8_t x = UDR0;
    TOGGLE_LED_PIN();
    UDR0 = x;
}

void setup_uart(){
    UBRR0H = (uint8_t)((UBRR & 0x0f00) >> 8);
    UBRR0L = (uint8_t)((UBRR & 0x00ff));
    /* enable tx, rx and rx int */
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
    /* enable double speed, 115200 doesn't work  */
    UCSR0A = (1 << U2X0);
    /* 8-bit, 2 stop bits*/
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}