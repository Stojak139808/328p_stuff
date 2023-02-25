#include "include/uart.h"
#include "include/gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

uint8_t _rx_buff[RX_BUFF_SIZE] = {0};

FIFO uart_data_buff = {
    .head = 0,
    .tail = 0,
    .size = RX_BUFF_SIZE,
    .data = _rx_buff
};

ISR(USART_RX_vect){
    uint8_t status = read_uart_to_buff();
    if( status == UART_ERROR )
        TOGGLE_LED_PIN();
}

void setup_uart(){
    UBRR0H = (uint8_t)((UBRR & 0x0f00) >> 8);
    UBRR0L = (uint8_t)((UBRR & 0x00ff));
    /* enable tx, rx and rx int */
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
    /* enable double speed, 115200 doesn't work  */
    UCSR0A |= (1 << U2X0);
    /* 8-bit, 2 stop bits*/
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

static uint8_t read_uart_to_buff(){
    uint8_t buff;
    uint8_t state;

    buff = UDR0;
    state = fifo_write(&uart_data_buff, &buff);

    if( state == FIFO_FULL )
        return UART_ERROR;

    return UART_SUCESS;
}

uint8_t fifo_read(FIFO *fifo, uint8_t *dest){
    if (fifo->head == fifo->tail) {
        return FIFO_EMPTY;
    }
    *dest = fifo->data[fifo->tail];
    fifo->data[fifo->tail] = 0;
    fifo->tail = (fifo->tail + 1) % fifo->size;
    return FIFO_SUCESS;
}

uint8_t fifo_write(FIFO *fifo, uint8_t *src){
    if (((fifo->head + 1) % fifo->size) == fifo->tail) {
        return FIFO_FULL;
    }
    fifo->data[fifo->head] = *src;
    fifo->head = (fifo->head + 1) % fifo->size;
    return FIFO_SUCESS; 
}

uint8_t read_uart_buff(char *destination){
    return 0;
}

int send_uart(char* message){
    return 0;
}
