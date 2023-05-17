/*
 * uart.h
 *
 * Created: 03.03.2023 14:47:05
 *  Author: fuxa9
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"

// frekvence cpu
#define F_CPU 16000000UL
// nastavenu baudrate
#define BAUD_RATE 38400
// vypocet ubrr hodnoty podle vzorce
#define UBRR_VALUE ((F_CPU / (16UL * BAUD_RATE)) - 1)


// funkce ktera zpracovava stdout
void stdout_putchar(char c, FILE *stream) {
	// kontrola zda znak je /n
	if (c == '\n') {
		uart_transmit('\r');
	}
	// pokud neni posila samotny znak
	uart_transmit(c);
}

void uart_init(){
	// nastaveni baudradu
	UBRR0 = UBRR_VALUE;

	// nastaveni formatu zpravy na 8 datych bitu, no parity, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	// povoleni prijimace a vysilace
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void uart_transmit(uint8_t data){
	// ceka dokud nebude vysilaci buffer prazdny
	while(!(UCSR0A & (1 << UDRE0)));

	// posle data
	UDR0 = data;
}

uint8_t uart_receive(){
	// pocka dokud neprijdou data
	while (!(UCSR0A & (1 << RXC0)));

	// precte data
	return UDR0;
}
