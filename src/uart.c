#include "include/uart.h"
#include "include/gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "include/debug.h"

static uint8_t read_uart_to_buff(void);
static uint8_t send_uart_from_buff(void);

uint8_t _rx_buff[RX_BUFF_SIZE] = {0};
uint8_t _tx_buff[TX_BUFF_SIZE] = {0};

UART_PORT rx = {
    .buff = {
        .head = 0,
        .tail = 0,
        .size = RX_BUFF_SIZE,
        .data = _rx_buff
    },
    .n_messages = 0
};

UART_PORT tx = {
    .buff = {
        .head = 0,
        .tail = 0,
        .size = TX_BUFF_SIZE,
        .data = _tx_buff
    },
    .n_messages = 0
};

ISR(USART_RX_vect){
    uint8_t status = read_uart_to_buff();
    if( status == UART_ERROR ){
        DEBUG("BUFFER FULL\n\r");
    }
}

ISR(USART_TX_vect){
    uint8_t status = send_uart_from_buff();
    if( status == UART_DONE )
        DISABLE_TX_ISR();
}

void setup_uart(){
    UBRR0H = (uint8_t)((UBRR & 0x0f00) >> 8);
    UBRR0L = (uint8_t)((UBRR & 0x00ff));
    /* enable tx, rx and rx int */
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
    /* enable double speed, 115200 doesn't work  */
    UCSR0A |= (1 << U2X0);
    /* 8-bit, 1 stop bits*/
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

static uint8_t read_uart_to_buff(){
    uint8_t buff;
    uint8_t state;

    buff = UDR0;
    state = fifo_write(&rx.buff, &buff);

    if( state == FIFO_FULL ){
        rx.n_messages++;
        return UART_ERROR;
    }

    if( buff == RX_UART_END_CHAR )
        rx.n_messages++;

    return UART_SUCESS;
}

static uint8_t send_uart_from_buff(){
    uint8_t buff, status;
    status = fifo_read(&tx.buff, &buff);

    if( status == FIFO_EMPTY ){
        tx.n_messages = 0;
        return UART_DONE;
    }

    UDR0 = buff;

    if( buff == TX_UART_END_CHAR ){
        tx.n_messages--;
        return UART_DONE;
    }

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
    uint8_t i, status;
    for( i = 0; i < RX_BUFF_SIZE; i++ ){
        status = fifo_read(&rx.buff, destination + i);
        if( status == FIFO_EMPTY ){
            rx.n_messages = 0;
            return i;
        }

        if( destination[i] == RX_UART_END_CHAR ){
            rx.n_messages--;
            return i;
        }
    }
    /* loop didnt stop => entire buffer was read */
    rx.n_messages = 0;
    return i;
}

uint8_t send_uart(char* message){
    uint8_t status, i;

    status = FIFO_SUCESS;
    i = 0;
    while( status == FIFO_SUCESS && i < TX_BUFF_SIZE && message[i] ){
        status = fifo_write(&tx.buff, message + i);
        i++;
    }
    tx.n_messages++;

    fifo_write(&tx.buff, '\0');

    ENABLE_TX_ISR();
    send_uart_from_buff();

    return UART_SUCESS;
}

uint8_t uart_rx_state(){
    return rx.n_messages;
}

uint8_t uart_tx_state(){
    return tx.n_messages;
}