#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

int main(void) {
	DDRB = 0xFF ^ (1 << DDB2);
	PORTB = 0x00;
	OCR1A = 60;
	TCCR1 = (1 << CTC1) | (1 << CS11) | (1 << CS12) | (1 << CS13);
	uint8_t buttonDown = 0;
	uint8_t on = 0;
	uint8_t halfSeconds = 0;
	
	cli();			
	GIMSK |= (1 << INT0);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	MCUCR &= ~(1 << ISC00);
	MCUCR |= (1 << ISC01);
	sei();

	for(;;) {
		if (bit_is_clear(PINB, PB2)) {
			if (buttonDown == 0) {
				TCCR1 = (1 << CTC1) | (1 << CS11) | (1 << CS12) | (1 << CS13);
				buttonDown = 1;
			} else {
				if (TIFR & (1 << OCF1A)) {
					halfSeconds += 1;
					TIFR = (1 << OCF1A);
					TCNT1 = 0;
				}
				if (halfSeconds == 2 && on == 0) {
					PORTB = (1 << PORTB1) | (1 << PORTB3);
					on = 1;
					halfSeconds = 0;
					TCCR1 = 0x00;
				} else if (halfSeconds == 3 && on == 1) {
					PORTB = 0x00;
					on = 0;
					halfSeconds = 0;
					TCCR1 = 0x00;
				}
			}
		} else if (bit_is_set(PINB, PB2)) {
			buttonDown = 0;
			halfSeconds = 0;
			TCCR1 = 0x00;
			
			sleep_mode();
		}
	}
	
	return 0;
}

ISR(INT0_vect) {
	//Check if needed
}
