
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
// Defines
#define LED (1<<5)

uint32_t actual = 0;

// globalni promenna na pocitani preteceni
volatile uint16_t overflowCount = 0;


ISR(TIMER1_OVF_vect)
{
	// pricteni preteceni citace
	++overflowCount;
}
void timerInit()
{
	// Vynulovani casovace
	TCNT1 = 0x00;
	// Smazani vlakej preruseni
	TIFR1 = (1<<TOV1) | (1<<OCF1A);
	// predelicka 8, tedy perioda 0.5 us, spustit casovac
	TCCR1B = 0x02;
	// povoleni preruseni casovace
	TIMSK1 |= (1 << TOIE1);
	sei();

}

uint32_t getTime(void)
{
	// vypnuti preruseni
	cli();
	
	// OverflowCount na top 15 bitu a zbytek ma aktualni cas, nutne pretypovat aby
	// nebyla ztracena informace
	actual = ((uint32_t)overflowCount << 15) | (TCNT1 >> 1);
	
	// opetovne zapnuti preruseni
	sei();

	return actual;
}

void delay(uint32_t us)
{
	// pricteni aktualniho casu k dobe cekani
	us += getTime();
	
	// cekaci smycka
	while(getTime() < us);
}
