/*
 * global.h
 *
 * Created: 28.04.2023 22:57:26
 *  Author: fuxa9
 */ 

#ifndef GLOBAL_H
#define GLOBAL_H

// definice pro presmerovani stdout na display
static int lcd_send_fprintf(char data, FILE* stream);
static FILE lcd_out = FDEV_SETUP_STREAM(lcd_send_fprintf, NULL, _FDEV_SETUP_WRITE);
static int lcd_send_fprintf(char data, FILE* stream){
	lcd_send_data(data);
	return 0;
}
#endif
