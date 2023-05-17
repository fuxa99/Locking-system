/*
 * relay.h
 *
 * Created: 25.04.2023 19:35:06
 *  Author: fuxa9
 */ 

#ifndef RELAY_H
#define RELAY_H

#define ALARM PORTD2
#define DOORS PORTD3

void relay_init(void);
void unlock_door(uint8_t rtc_data[7]);
void lock_door(uint8_t rtc_data[7]);
void start_alarm(uint8_t rtc_data[7]);
void stop_alarm(uint8_t rtc_data[7]);
void check_lock_unlock_time(uint8_t rtc_data[7], int *relay[2], int unlock_lock_time[4]);
uint8_t bcd2dec(uint8_t val);

#endif
