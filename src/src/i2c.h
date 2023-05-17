/*
 * RTC.h
 *
 * Created: 30.03.2023 14:43:50
 *  Author: fuxa9
 */ 


#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <stdint.h>
#include <string.h>
#include <avr/interrupt.h>

#define TWI_TIMEOUT 1600
#define F_CPU 16000000UL

#define TWI_START      0x08
#define TWI_RSTART     0x10
#define TWIT_ADDR_ACK  0x18
#define TWIT_ADDR_NACK 0x20
#define TWIT_DATA_ACK  0x28
#define TWI_DATA_NACK  0x30

#define TWIR_ADDR_ACK  0x40
#define TWIR_ADDR_NACK 0x48
#define TWIR_DATA_ACK  0x50
#define TWIR_DATA_NACK 0x58

#define TWI_ERROR      0x38
#define TWI_NONE       0xF8

enum{
	TWI_OK,
	TWI_ERROR_START,
	TWI_ERROR_RSTART, 
	TWI_NACK
};

void twi_init(uint32_t speed);
uint8_t twi_wire(uint8_t addr,uint8_t reg,uint8_t *data,uint16_t len);
uint8_t twi_read(uint8_t addr,uint8_t reg,uint8_t *data,uint16_t len);

#endif
