/*
 * display.c
 *
 * Created: 3/16/2023 9:00:00 AM
 *  Author: fuxa9
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include "timer.h"
#include "display.h"
#include "ADC.h"



void LCD_Init(void){
	
	// Nastaveni vystupu datove a RS a EN
	DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);
	DDRB |= (1 << DDB0) | (1 << DDB1);
	
	PORTD &= 0;
	PORTB &= ~(1 << PINB1);
	PORTB &= ~(1 << PINB0);
	
	// Software reset
	delay(20000);
	write_4b(0x03);
	delay(5000);
	write_4b(0x03);
	delay(110);
	write_4b(0x03);
	delay(3000);
	
	write_4b(0x02);
	delay(3000);
	lcd_send_command(0x28); //Parametr displeje
	delay(3000);
	lcd_send_command(0x0c); //Zapnuti displeje
	delay(3000);
	lcd_send_command(0x01); //Mazani displeje
	delay(3000);
	
}

char* readButton()
{
	uint16_t result_adc0 = read_adc(0); // cteni hodnoty z ADC0
	int ret_value = 0;
	
	if (result_adc0 > 90 && result_adc0 < 100) ret_value =  1;
	else if (result_adc0 > 200 && result_adc0 < 280) ret_value = 2;
	else if (result_adc0 > 400 && result_adc0 < 500) ret_value = 3;
	else if ( result_adc0 > 600 && result_adc0 < 700) ret_value = 5;
	else if ( result_adc0 < 100) ret_value = 4;
	else ret_value = 0;
	// cekani na rozepnuti tlacitka
	while(read_adc(0) < 1000) delay(30000);
	return ret_value;
}

void write_4b(unsigned char prikaz){
	
	PORTD = (PORTD & 0x0F) | (prikaz << 4);
	PORTB |= (1 << PINB1);                    //EN = 1
	delay(1);
	PORTB &= ~(1 << PINB1);					 //EN = 0
	delay(100);
	
}

void lcd_send_command(unsigned char prikaz){
	// funkce pro displej
	PORTB &= ~(1 << PINB0);    //RS = 0
	write_4b(prikaz >> 4);
	write_4b(prikaz & 0x0F);
	
}

void lcd_send_data(unsigned char znak){
	//Funkce pro posilani jednoho znaku
	PORTB |= (1 << PINB0);    //RS = 1
	write_4b(znak >> 4);
	write_4b(znak & 0x0F);
}

void lcd_write_string(char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}
void lcd_clear() {
	lcd_send_command(0x01);
	delay(2000); // poèkat na dokonèení operace
}

void lcd_home() {
	lcd_send_command(0x02);
	delay(2000); // poèkat na dokonèení operace
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54}; // adresa øádkù
	lcd_send_command(0x80 | (row_offsets[row] + col)); // nastavit adresu øádku a sloupce
	delay(50); // poèkat na dokonèení operace
}

void lcd_scroll_left() {
	lcd_send_command(0x18);
	delay(50); // poèkat na dokonèení operace
}

void lcd_scroll_right() {
	lcd_send_command(0x1C);
	delay(50); // poèkat na dokonèení operace
}

void lcd_create_char(uint8_t location, uint8_t *charmap) {
	location &= 0x7; // omezení umístìní znaku na 8 poloh
	lcd_send_command(0x40 | (location << 3));
	for (int i = 0; i < 8; i++) {
		lcd_send_data(charmap[i]);
	}
}

void lcd_write_custom_char(uint8_t location) {
	lcd_send_data(location);
}

void lcd_shift_cursor_left() {
	lcd_send_command(0x10);
	delay(50); // poèkat na dokonèení operace
}

void lcd_shift_cursor_right() {
	lcd_send_command(0x14);
	delay(50); // poèkat na dokonèení operace
}

void lcd_shift_display_left() {
	lcd_send_command(0x18);
	delay(50); // poèkat na dokonèení operace
}

void lcd_shift_display_right() {
	lcd_send_command(0x1C);
	delay(50); // poèkat na dokonèení operace
}