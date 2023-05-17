/*
 * user.c
 *
 * Created: 28.04.2023 20:43:41
 *  Author: fuxa9
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <avr/interrupt.h>

#include "timer.h"
#include "ADC.h"
#include "uart.h"
#include "keypad.h"
#include "relay.h"
#include "display.h"
#include "global.h"
#include "relay.h"

uint8_t MAX_BUFFER_LEN = 16;

int increment = 0;
int strikes = 0;
char buffer[16];
char pass_buffer[16];

int check_password(int password,  int *relay[2]){
	increment = 0; 
	strikes = 0; // pocet spatnych pokusu
	delay(100000);
	
	while(1)
	{ // smycka pro pocitani chyb
		lcd_clear();
		lcd_home();
		fprintf(&lcd_out, "Write pass: at:%d", strikes);
		lcd_set_cursor(1,0);	
		memset(buffer, 0, sizeof(buffer));
		readButton_buffer(12, 3); // napsani hesla o velikosti 12 a 3 stav pro vypisovani
		int i, result = 0;
	
		// prevedeni z charu na jedno cislo
		for (i = 0; buffer[i] != '\0'; i++) {
			result = result * 10 + (buffer[i] - '0'); // prevedeni znaku na cislo
		}
		if (result == password)
		{
			return(1); // je-li heslo spravne ukoncit tuto funkci
		}else strikes++; // pokud neni sparvne heslo pricti strike
		if (strikes >= 3)
		{
			return(0);
		}
	}
	
}

void set_unlock_time(int* unlock_lock_time[4], uint8_t rtc_data[7])
{
	// vymazani obsahu pole
	memset(buffer, 0, sizeof(buffer));
	increment = 0;
	lcd_clear();
	lcd_home();
	fprintf(&lcd_out, "Set hour xx:");
	lcd_set_cursor(1,0);
	delay(100000);
	
	readButton_buffer(2, 1);
	int i, result = 0;
	
	// prevedeni z charu na jedno cislo
	for (i = 0; buffer[i] != '\0'; i++) {
		result = result * 10 + (buffer[i] - '0');
	}
	unlock_lock_time[0] = result; // ulozeni hodin pro unlock time
	
	memset(buffer, 0, sizeof(buffer));
	increment = 0;
	lcd_clear();
	lcd_home();
	fprintf(&lcd_out, "Set minutes xx:");
	lcd_set_cursor(1,0);
	delay(100000);
	readButton_buffer(2, 2);
	//printf("Do bufferu nacteny hodnoty %c a %c \n",buffer[0],buffer[1]);
	result = 0;
	
	for (i = 0; buffer[i] != '\0'; i++) {
		result = result * 10 + (buffer[i] - '0');
	}
	unlock_lock_time[1] = result; // ulozeni minut pro unlock time
	printf("20%02x/%02x/%02x %02x:%02x:%02x - Unlock time nastaven na %d:%d \n",rtc_data[6],rtc_data[5],rtc_data[4], rtc_data[2], rtc_data[1], rtc_data[0], unlock_lock_time[0], unlock_lock_time[1]);
}

void set_lock_time(int* unlock_lock_time[4], uint8_t rtc_data[7]){
	memset(buffer, 0, sizeof(buffer));
	increment = 0;
	lcd_clear();
	lcd_home();
	fprintf(&lcd_out, "Set hour xx:");
	lcd_set_cursor(1,0);
	delay(100000);
	
	readButton_buffer(2, 1);
	//printf("Do bufferu nacteny hodnoty %c a %c \n",buffer[0],buffer[1]);
	int i, result = 0;
	
	for (i = 0; buffer[i] != '\0'; i++) {
		result = result * 10 + (buffer[i] - '0');
	}
	unlock_lock_time[2] = result; // ulozeni hodin pro lock time
	
	memset(buffer, 0, sizeof(buffer));
	increment = 0;
	lcd_clear();
	lcd_home();
	fprintf(&lcd_out, "Set minutes xx:");
	lcd_set_cursor(1,0);
	delay(100000);
	readButton_buffer(2, 2);
	//printf("Do bufferu nacteny hodnoty %c a %c \n",buffer[0],buffer[1]);
	result = 0;
	
	for (i = 0; buffer[i] != '\0'; i++) {
		result = result * 10 + (buffer[i] - '0');
	}
	unlock_lock_time[3] = result; // ulozeni minut pro lock time
	// vypsani vysledku
	printf("20%02x/%02x/%02x %02x:%02x:%02x - Lock time nastaven na %d:%d \n",rtc_data[6],rtc_data[5],rtc_data[4], rtc_data[2], rtc_data[1], rtc_data[0], unlock_lock_time[0], unlock_lock_time[1]);
}


void readButton_buffer(int leng_of_increment, int statemant){
	while (1){
		int lcd_button = readButton();
		// Cteni jake tlacitko bylo stisknuto
		char get_key =  matrix_keypad_getkey();
		if (get_key != '\0' && increment < leng_of_increment)
		{
			// vypsani na display a ulozeni do bufferu
			buffer[increment] = get_key;
			lcd_set_cursor(1,0);
			fprintf(&lcd_out, (char*)buffer);
			increment++;
		}
		else if (lcd_button == 5)
		{
			// vymazani bufferu aby se mohlo zacit psat znova
			lcd_clear();
			lcd_home();
			switch(statemant){
				case 1:
					fprintf(&lcd_out, "Set hour xx:");
					break;
				case 2:
					fprintf(&lcd_out, "Set minutes xx:");
					break;
				case 3:
					fprintf(&lcd_out, "Write pass: at:%d", strikes);
			}
			memset(buffer, 0, sizeof(buffer));
			increment = 0;
		}
		if(lcd_button == 4){
			// ukonceni cteni z klavesnice pokud se stiklo tlacitko RIGHT na diplaji
			break;
		}
		delay(50);
	}
}