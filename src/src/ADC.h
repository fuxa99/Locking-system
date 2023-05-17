/*
 * ADC.h
 *
 * Created: 08.03.2023 22:13:00
 *  Author: fuxa9
 */ 

#ifndef ADC_H
#define ADC_H

void ADC_init(void);
uint16_t read_adc(uint8_t adc_channel);
float get_Temperature(uint8_t adc_channel);

#endif