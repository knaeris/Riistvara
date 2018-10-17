#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000


void main(void)
{
    DDRA |= _BV(PA1);
    DDRA |= _BV(PA3);
    DDRA |= _BV(PA5);
    DDRB |= _BV(DDB7);
    PORTB &= ~_BV(PORTB7);

    while (1) {
        PORTA |= _BV(PA1);
        _delay_ms(BLINK_DELAY_MS);
        PORTA &= ~_BV(PA1);
        _delay_ms(BLINK_DELAY_MS);
        PORTA |= _BV(PA3);
        _delay_ms(BLINK_DELAY_MS);
        PORTA &= ~_BV(PA3);
        _delay_ms(BLINK_DELAY_MS);
        PORTA |= _BV(PA5);
        _delay_ms(BLINK_DELAY_MS);
        PORTA &= ~_BV(PA5);
        _delay_ms(BLINK_DELAY_MS);
    }
}

