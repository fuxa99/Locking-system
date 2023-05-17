/*
 * timer.h
 *
 * Created: 01.03.2023 23:59:51
 *  Author: fuxa9
 */ 

#ifndef TIMER_H
#define TIMER_H

void timerInit(void);
uint32_t getTime(void);

void delay(uint32_t);
void busyDelay(uint32_t);

#endif /* TIMER_H */