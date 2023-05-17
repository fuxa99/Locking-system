/*
 * keypad.c
 *
 * Created: 3/19/2023 1:14:21 PM
 *  Author: fuxa9
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "keypad.h"
#include "timer.h"

int wait_time = 30000;

// klavesnice, kompletni
char keys[ROWS][COLS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

void matrix_keypad_init(void) {
	  // nastaveni radku jako vystup
	  DDRB |= (1 << PINB3) | (1 << PINB4) | (1 << PINB5);
	  
	  // nastaveni sloupcu jako vstup
	  DDRC &= ~((1 << PINC1) | (1 << PINC2) | (1 << PINC3));
	  
	  // zapnuti pullup
	  PORTC |= (1 << PINC1) | (1 << PINC2) | (1 << PINC3);
	  
	  // nastaveni radku do HIGH
	  //PORTB |= (1 << PINB2);
	  PORTB |= (1 << PINB3);
	  PORTB |= (1 << PINB3);
	  PORTB |= (1 << PINB4); 
}

char matrix_keypad_getkey(void) {
    char key = '\0';
	for (int i = 0; i < ROWS - 1; i++)
	{
		for (int j = 0; j < COLS - 1; j++)
		{
			// nastaveni konkretniho radku na LOW
			PORTB &= ~(1 << (PINB3 + i));
			
			delay(wait_time);
			// kontrola jake je stiknute tlacitko
			if (!(PINC & (1 << (PINC1 + j))))
			{
				key = keys[i][j];
				// cekani na rozepnuti tlacitka
				while(!(PINC & (1 << (PINC1 + j))))
				{
					// delay na osetreni zakmitu
					delay(wait_time);
				}
				// vyzkoceni z foru
                break;
			}
			// nastaveni konkretniho radku na HIGH
			PORTB |= (1 << (PINB3 + i));
			
			delay(wait_time);
		}
		// pokud bylo stiknute tlaxitko vyzkoc z foru
        if (key != '\0') {
            break;
        }
    }
    return key;
}