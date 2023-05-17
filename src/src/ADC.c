/*
 * ADC.c
 *
 * Created: 08.03.2023 22:12:48
 *  Author: fuxa9
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include "ADC.h"

const float T0 = 298.15;  // teplota 25 stupnu C v kelvinech
const float B = 3950.0;   // B-koeficient termistoru
const float R0 = 10000.0; // odpor termistoru pøi teplotì T0
const float R3 = 1500.0; // odpor R3

char buffer[10];

void ADC_init(void)
{
	ADMUX |= (1 << REFS0); // nastaveni reference napeti na AVCC
	ADMUX &= ~(MUX3 | MUX2 | MUX1 | MUX0); // Vynulovani ADC
	ADCSRA |= (1 << ADEN); // povolení ADC
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // nastaveni preddelicky na 128 (125 kHz), pouziva se u termisotru, zvysuje cistlivost
}

uint16_t read_adc(uint8_t adc_channel) {
	// Nastavení ADMUX pro zvolený kanál ADC
	ADMUX &= ~(MUX3 | MUX2 | MUX1 | MUX0);
	ADMUX |= adc_channel;

	// Spuštìní pøevodu ADC
	ADCSRA |= (1 << ADSC);

	// Èekání na dokonèení pøevodu ADC
	while (ADCSRA & (1 << ADSC));

	// Ètení výsledku z ADC
	uint16_t result_adc = ADC;

	return result_adc;
}

float get_Temperature(uint8_t adc_channel)
{
	int adc_value =  read_adc(adc_channel); // hodnota z AD prevodniku
	float voltage = adc_value * 5.0 / 1024.0; // prepocet na napeti
	
	float Rth = voltage/(5.0 - voltage) * 20000.0 - R3; // vypocet odporu termistoru
    float T = B/(log(Rth / R0) + B/T0); // vypocet velikost teploty v kelvinech
    return T - 273.15; // pøevod na stupnì C
}