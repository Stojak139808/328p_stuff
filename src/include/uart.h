#ifndef UART_H
#define UART_H

#include "mcu.h"

/* USART Macros*/
#define BAUD 115200u
#define UBRR F_CPU/8u/BAUD-1

void setup_uart(void);

#endif