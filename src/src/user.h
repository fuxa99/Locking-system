/*
 * user.h
 *
 * Created: 28.04.2023 20:43:50
 *  Author: fuxa9
 */ 

#ifndef USER_H
#define USER_H

void add_user(void);
void remove_user(void);
int check_password(int password,  int *relay[2]);

void set_lock_time(int* unlock_lock_time[4], uint8_t rtc_data[7]);
void set_unlock_time(int* unlock_lock_time[4], uint8_t rtc_data[7]);
#endif
