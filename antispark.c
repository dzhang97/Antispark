#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

int main(void) {
	ADCSRA &= ~(1 << ADEN);
	
	DDRB = 0xFF ^ (1 << DDB4);
	PORTB = 0x00;
	OCR1A = 60;
	TCCR1 = (1 << CTC1) | (1 << CS11) | (1 << CS12) | (1 << CS13);
	uint8_t buttonDown = 0;
	uint8_t on = 0;
	uint8_t halfSeconds = 0;
	
	cli();			
	GIMSK |= (1 << PCIE);
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	PCMSK |= (1 << PCINT4);
	sei();

	for(;;) {
		if (bit_is_clear(PINB, PB4)) {
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
					PORTB = (1 << PORTB2) | (1 << PORTB3);
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
		} else if (bit_is_set(PINB, PB4)) {
			buttonDown = 0;
			halfSeconds = 0;
			TCCR1 = 0x00;
			
			sleep_mode();
		}
	}
	
	return 0;
}

ISR(PCINT0_vect) {
	//Check if needed
}
