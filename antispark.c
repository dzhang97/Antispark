#include <avr/io.h>

int main(void) {
	DDRB = 0xFF ^ (1<<PIN4);
	PORTB = 0x00 | (1 << PORTB4);
	OCR1A = 60;
	TCCR1 = (1 << CTC1) | (1 << CS11) | (1 << CS12) | (1 << CS13);
	uint8_t buttonDown = 0;
	uint8_t on = 0;
	uint8_t halfSeconds = 0;

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
		}
	}
	
	return 0;
}
