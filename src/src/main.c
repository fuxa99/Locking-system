/*
 * display.c
 *
 * Created: 3/16/2023 9:00:00 AM
 *  Author: fuxa9
 */ 
// import externich knihoven
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <avr/interrupt.h>

// import vlastnich knihoven
#include "timer.h"
#include "display.h"
#include "ADC.h"
#include "uart.h"
#include "i2c.h"
#include "keypad.h"
#include "relay.h"
#include "global.h"

#define RTC_ADRESS 0x68 // adresa RTC
static char print_buffer[64] = {0}; // buffer pro vypis erroru
uint8_t rtc_data[7] = {0x10, 0x56, 0x12, 0x07, 0x28, 0x04, 0x23}; // set time - second,minutes,hours,day,day of week,day,month,year
	
// definice pro presmerovani stdout na uart
extern FILE uart_output = FDEV_SETUP_STREAM(stdout_putchar, NULL, _FDEV_SETUP_WRITE);

// variables
int relay[2] = {0,0}; // Signalizace stavu relatek
int unlock_lock_time[4] = {7,30,19,30}; // Prvotni nastaveni lock a unlock casu
uint8_t err = 0;
int main_password = 123456;

// functions
void update_time(void);

int main(void)
{
	timerInit();
	uart_init();
	ADC_init();
	LCD_Init();
	twi_init(100000); // 100khz
	relay_init();
	matrix_keypad_init();
	
	// presmerovani printf na uart
	stdout = &uart_output;
	
	//struct user admin;
	//admin.name = "admin";
	//admin.password = 123456;

	// set time to RTC
    err = twi_wire(RTC_ADRESS, 0x00, rtc_data, sizeof(rtc_data));
    if(err != TWI_OK)
    {
	    memset(print_buffer, 0, sizeof(print_buffer));
	    sprintf(print_buffer, "%d error %d \r\n\n", __LINE__,err);
	    printf((uint8_t*)print_buffer);
	    while(1);
    }

	
	fprintf(&lcd_out, "Dominik Fuxa");
	printf("Dominik Fuxa\n");
	lcd_set_cursor(1,0);
	fprintf(&lcd_out, "Lock system 2023");
	printf("Lock system 2023\n");
	delay(5000000);
	lcd_clear();
	lcd_home();
	
	sei();
	
	// defaultne nastaveni pri zapnuti systemu
	lock_door(&rtc_data);
	relay[0] = 0;
	stop_alarm(&rtc_data);
	relay[1] = 0;
	
	int counter[2] = {1,0}; // promena slouzici pro urceni zda se ma neco prepsat na displaji nebo ne

	while(1)
	{
		int lcd_button = readButton();
		switch(lcd_button){
			case 2: // DOWN button
				counter[0]++; // pokud je stiknuto tlacitko dolu pricte hodnotu
				break;
			case 1: // UP button
				counter[0]--; // pokud je stiknuto tlacitko nahoru odecte hodnotu
				break;
			case 4: // RIGHT button
				switch(counter[0]){
					case 1: // ADD user case
						counter[1] = 0;
						break;
					case 2: // Remove user case
						counter[1] = 0;
						break;
					case 3: // Unlock door case
						if (relay[0] == 0) // test zda nejsou dvere jiz otevrene
						{
							if (check_password(main_password, &relay) == 1)
							{
								lcd_clear();
								lcd_home();
								fprintf(&lcd_out, "Unlocking doors");
								lcd_set_cursor(1,4);
								fprintf(&lcd_out, "to 5 sec");
								unlock_door(rtc_data);
								delay(5000000);
								update_time();
								lock_door(rtc_data);
								relay[0] = 0;
							}else{
								lcd_clear();
								lcd_home();
								fprintf(&lcd_out, "Access denied");
								delay(1000000);
								if (relay[1] == 1)
								{
									printf("Zadani spatneho hesla vyhlaseni alarmu! \n");
								}
							}
							
						}else{
							lcd_clear();
							lcd_home();
							fprintf(&lcd_out, "Door are already");
							lcd_set_cursor(1,5);
							fprintf(&lcd_out, "unlock");
							delay(3000000);
						}
						counter[1] = 0;
						break;
					case 4: // Lock time case
						set_lock_time(&unlock_lock_time, &rtc_data);
						counter[1] = 0;
						break;
					case 5: // Unlock time case
						set_unlock_time(&unlock_lock_time, &rtc_data);
						counter[1] = 0;
						break;
					case 6: // Alarm On case
						lcd_clear();
						if (relay[1] != 1){ // test zda neni jiz alarm zapnut
							start_alarm(&rtc_data);
							lcd_set_cursor(0,2);
							fprintf(&lcd_out, "ALARM is ON");
							delay(1000000);
							relay[1] = 1;
						}else{
							lcd_home();
							fprintf(&lcd_out, "Alarm is already");
							lcd_set_cursor(1,7);
							fprintf(&lcd_out, "ON");
							delay(1000000);
						}

						counter[1] = 0;
						break;
					case 7: // Alarm OFF case
						lcd_clear();lcd_clear();
						if (relay[1] != 0) // test zda jiz neni alarm vypunut
						{
							if (check_password(main_password, &relay) == 1){
								stop_alarm(&rtc_data);
								lcd_clear();
								lcd_set_cursor(0,2);
								fprintf(&lcd_out, "ALARM is OFF");
								delay(1000000);						
								relay[1] = 0;	
							}
						}else{
							lcd_home();
							fprintf(&lcd_out, "Alarm is already");
							lcd_set_cursor(1,7);
							fprintf(&lcd_out, "OFF");
							delay(1000000);						
						}
						counter[1] = 0;
						break;						
					case 8: // Show loc/unl case
						lcd_clear();
						lcd_home();
						fprintf(&lcd_out, "Unlock %d:%d", unlock_lock_time[0], unlock_lock_time[1]);
						lcd_set_cursor(1,0);
						fprintf(&lcd_out, "Lock %d:%d", unlock_lock_time[2], unlock_lock_time[3]);
						delay(3000000);
						counter[1] = 0;
						break;
				}
				break;
		}
		if (counter[0] < 1) counter[0] = 1;
		else if (counter[0] > 8) counter[0] = 8; // zabranuje prekroceni hodnoty pro menu
		
		if(counter[0] != counter[1]){
			lcd_clear();
			lcd_home();
			switch(counter[0]){
				case 1:
					lcd_set_cursor(0,0);
					fprintf(&lcd_out,"1: ADD user");
					break;
				case 2:
					lcd_set_cursor(0,0);
					fprintf(&lcd_out,"2: Remove user");
					break;
				case 3:
					lcd_set_cursor(0,0);
					fprintf(&lcd_out,"3: Unlock door");
					break;
				case 4:
					lcd_set_cursor(0,0);
					fprintf(&lcd_out,"4: Lock time");
					break;
				case 5:
					lcd_set_cursor(0,0);
					fprintf(&lcd_out,"5: Unlock time");
					break;
				case 6:
					lcd_set_cursor(0,0);
					fprintf(&lcd_out, "6: Alarm ON");
					break;
				case 7:
					lcd_set_cursor(0,0);
					fprintf(&lcd_out, "7: Alarm OFF");
					break;
				case 8:
					lcd_set_cursor(0,0);
					fprintf(&lcd_out, "8: Show loc/unl");
					break;
			}
			lcd_set_cursor(1,0);
			lcd_write_custom_char(0x7E); // -> vykresleni sipkz
			fprintf(&lcd_out,"IN");
		
			counter[1] = counter[0]; // updatep promenne aby uz nedochazelo k vypisovani na displej
		}

		if (relay[1] == 1)
		{
			lcd_set_cursor(0,15);
			lcd_write_custom_char(0xCE); // CE indikace zapnuteho alarmu
		}else {
			lcd_set_cursor(0,15);
			fprintf(&lcd_out, " "); 
		}
		
		check_lock_unlock_time(&rtc_data, &relay, &unlock_lock_time); // kontrola zda maji byt dvere automaticky otevreny nebo uzamceny
		lcd_set_cursor(1,5); // kurzor kde se vypisuje cas
		update_time(); // aktualizace casu
		fprintf(&lcd_out, "%02x/%02x %02x:%02x", rtc_data[4], rtc_data[5], rtc_data[2], rtc_data[1]); // vypis casu na display
		delay(100000);
	} // end of while
	return 0;
}

void update_time(void){
		err = twi_read(RTC_ADRESS, 0x00, rtc_data, sizeof(rtc_data)); // cteni z RTC data jsou nasledne ulozena do rtc_data
		if(err != TWI_OK) // kontrola zda pri cteni nenastala chyba
		{
			memset(print_buffer, 0, sizeof(print_buffer)); // vymazani bufferu
			sprintf(print_buffer, "%d error %d \r\n\n", __LINE__,err); // vypsani erroru po uartu
			printf((uint8_t*)print_buffer); // vypsani bufferu na uart
			//while(1); //nekonecna smycka
		}
}
