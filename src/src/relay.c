/*
 * relay.c
 *
 * Created: 25.04.2023 19:34:56
 *  Author: fuxa9
 */ 

#include <avr/io.h>
#include "relay.h"
#include "timer.h"

void relay_init(){
	DDRD |= (1 << ALARM); // nastevni vystupu
	DDRD |= (1 << DOORS); // nastaveni vystupu
}


void unlock_door(uint8_t rtc_data[]){ // odemceni dveri
	printf("20%02x/%02x/%02x %02x:%02x:%02x - Doors are unlock \n",rtc_data[6],rtc_data[5],rtc_data[4], rtc_data[2], rtc_data[1], rtc_data[0]); // vypis na UART
	PORTD |= (1 << DOORS); // odemceni dveri
}

void lock_door(uint8_t rtc_data[]){ // zamceni dveri
	printf("20%02x/%02x/%02x %02x:%02x:%02x - Doors are lock \n",rtc_data[6],rtc_data[5],rtc_data[4], rtc_data[2], rtc_data[1], rtc_data[0]); // vypis na UART
	PORTD &= ~(1 << DOORS); // zamceni dveri
}

void start_alarm(uint8_t rtc_data[]){
	printf("20%02x/%02x/%02x %02x:%02x:%02x - Alarm is ON \n",rtc_data[6],rtc_data[5],rtc_data[4], rtc_data[2], rtc_data[1], rtc_data[0]); // vypis na UART
	PORTD &= ~(1 << ALARM); // spusteni alarmu
}

void stop_alarm(uint8_t rtc_data[]){
	printf("20%02x/%02x/%02x %02x:%02x:%02x - Alarm is OFF \n",rtc_data[6],rtc_data[5],rtc_data[4], rtc_data[2], rtc_data[1], rtc_data[0]); // vypis na UART
	PORTD |= (1 << ALARM); // vypnuti alarmu
}

void check_lock_unlock_time(uint8_t rtc_data[], int* relay[2], int unlock_lock_time[4]){
	
	
	uint8_t rtc_minutes = bcd2dec(rtc_data[1]);
	uint8_t rtc_hours = bcd2dec(rtc_data[2]);
	//printf(unlock_lock_time[2]);
	//printf("\n");
	//printf("%d:%d and unlock time is %d:%d relay stav %d %d\n",rtc_hours, rtc_minutes, unlock_lock_time[0], unlock_lock_time[1], relay[0], relay[1]);
	
	if(unlock_lock_time[0] <= rtc_hours && rtc_hours <= unlock_lock_time[2] && relay[0] == 0)
	{	
		if (rtc_hours == unlock_lock_time[0] && rtc_minutes >= unlock_lock_time[1]){
			if(rtc_hours == unlock_lock_time[2]){
				if (rtc_minutes < unlock_lock_time[3])
				{
					relay[0] = 1;
					printf("Automaticke odemceni 1 \n");
					unlock_door(rtc_data);
				}
			}else
			{
				relay[0] = 1;
				printf("Automaticke odemceni 2 \n");
				unlock_door(rtc_data);
			}
			
		}else if (rtc_hours != unlock_lock_time[0])
		{
			if(rtc_hours == unlock_lock_time[2]){
				if (rtc_minutes < unlock_lock_time[3])
				{
					relay[0] = 1;
					printf("Automaticke odemceni 3 \n");
					unlock_door(rtc_data);
				}
			}else
			{
				relay[0] = 1;
				printf("Automaticke odemceni 4 \n");
				unlock_door(rtc_data);
			}		
		}
	}else if (rtc_hours >= unlock_lock_time[2] && rtc_minutes >= unlock_lock_time[3] && relay[0] == 1)
	{
		relay[0] = 0;
		printf("Automaticke zamceni \n");
		lock_door(rtc_data);
	}
	
}

// converts BCD number to decimal one
uint8_t bcd2dec(uint8_t val)
{
	return val - 6 * (val >> 4); 
}
