#ifndef UART_H
#define UART_H

#include "mcu.h"
#include <stdint.h>
#include <stdlib.h>

enum uart_status{
    UART_SUCESS,
    UART_ERROR,
};

enum fifo_status{
    FIFO_EMPTY,
    FIFO_FULL,
    FIFO_SUCESS
};

/* USART Macros*/
#define BAUD 115200u
#define UBRR F_CPU/8u/BAUD-1
#define RX_BUFF_SIZE 40u

typedef struct{
    uint8_t head;
    uint8_t tail;
    uint8_t size;
    uint8_t *data;
}FIFO;

void setup_uart(void);
static uint8_t read_uart_to_buff(void);
uint8_t read_uart_buff(char *destination);
uint8_t fifo_write(FIFO *fifo, uint8_t *src);
uint8_t fifo_read(FIFO *fifo, uint8_t *dest);

#endif