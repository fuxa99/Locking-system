/*
 * display.h
 *
 * Created: 3/16/2023 9:00:00 AM
 *  Author: fuxa9
 */ 
#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include "timer.h"

void LCD_Init(void);
void write_4b(unsigned char prikaz);
char* readButton();
void lcd_send_data(unsigned char znak);
void lcd_write_string(char *str);
void lcd_send_command(unsigned char prikaz);
void lcd_home(); // nastaveni cursoru na 0,0
void lcd_set_cursor(uint8_t row, uint8_t col); // nastaveni cursoru na vlasrni koordinaty
void lcd_scroll_left(); // scroll lcd do leva
void lcd_scroll_right(); // scroll lcd do prava
void lcd_create_char(uint8_t location, uint8_t *charmap); // vytvoreni vlastniho znaku
void lcd_write_custom_char(uint8_t location); // vypsani znaku z database
void lcd_shift_cursor_left(); // shiftnout cursor vlevo
void lcd_shift_cursor_right(); // shiftnout cursor vpravo
void lcd_shift_display_left();
void lcd_shift_display_right();
void lcd_clear(); // clear display


#endif 