#ifndef UART_H
#define UART_H

#include "mcu.h"
#include <stdint.h>
#include <stdlib.h>

enum uart_status{
    UART_SUCESS,
    UART_ERROR,
    UART_DONE
};

enum fifo_status{
    FIFO_EMPTY,
    FIFO_FULL,
    FIFO_SUCESS
};

#define BAUD 115200u
#define UBRR F_CPU/8u/BAUD-1
#define RX_BUFF_SIZE 64u
#define TX_BUFF_SIZE 64u

#define TX_UART_END_CHAR '\0'
#define RX_UART_END_CHAR '\r'

typedef struct{
    uint8_t head;
    uint8_t tail;
    uint8_t size;
    uint8_t* data;
}FIFO;

typedef struct{
    uint8_t size;
    uint8_t* data;
}BUFF;

enum{
    RX_EMPTY,
    RX_MESSAGE
};

enum{
    TX_EMPTY,
    TX_MESSAGE
};

enum{
    SEND_NORMAL,
    SEND_FORCE
};

typedef struct{
    FIFO buff;
    uint8_t n_messages;
}UART_PORT;

#define ENABLE_TX_ISR()     UCSR0B |= (1 << TXCIE0)
#define DISABLE_TX_ISR()    UCSR0B &= ~(1 << TXCIE0)

/* fifo functions */
uint8_t fifo_write(volatile FIFO *fifo, uint8_t *src);
uint8_t fifo_read(volatile FIFO *fifo, uint8_t *dest);

/* functions to use UART */
void setup_uart(void);
uint8_t read_uart_buff(char *destination);
uint8_t send_uart(char* message, uint8_t force);
uint8_t uart_rx_state(void);
uint8_t uart_tx_state(void);

#endif
