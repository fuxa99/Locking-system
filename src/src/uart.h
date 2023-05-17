/*
 * uart.h
 *
 * Created: 03.03.2023 14:47:05
 *  Author: fuxa9
 */ 

#ifndef UART_H
#define UART_H

// funkce ktera zpracovava stdout
void stdout_putchar(char c, FILE *stream);
// funkce pro inicializaci uart rozhrani
void uart_init();
// funkce pro posilani zpravy
void uart_transmit(uint8_t data);
// funkce pro prijimani zpravy
uint8_t uart_receive();

#endif
