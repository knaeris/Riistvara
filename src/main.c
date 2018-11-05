#include <stdio.h>
#define __ASSERT_USE_STDERR
#include <assert.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../lib/eriks_freemem/freemem.h"
#include "uart.h"

#define BLINK_DELAY_MS 100


static inline void init_leds(void)
{
    DDRA |= _BV(PA1);
    DDRA |= _BV(PA3);
    DDRA |= _BV(PA5);
    DDRB |= _BV(DDB7);
    PORTB &= ~_BV(PORTB7);
}

static inline void init_errcon(void)
{
    simple_uart1_init();
    stderr = &simple_uart1_out;
    assert(stderr != NULL);
    fprintf(stderr, "Version: " FW_VERSION " built on: " __DATE__ " "__TIME__ "\n");
    fprintf(stderr, "avr-libc version: " __AVR_LIBC_VERSION_STRING__ " "
            "avr-gcc version: " __VERSION__ "\n");
}
static inline void blink_leds(void)
{
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


void main(void)
{
    init_leds();
    init_errcon();
    /* Test assert - REMOVE IN FUTURE LABS */
    char *array = NULL;
    uint32_t i = 1;
    /*
     * As sizeof(char) is guaranteed to be 1 it is left here only for better
     * understanding how malloc works. No need to add it in production.
     */
    array = malloc( i * sizeof(char));

    if (array == NULL) {
        // Always test if malloc succeeds
        assert(array != NULL); // Fire assert and print error to stderr
        return; // Exit from program anyway when assert is disabled with NDEBUG.
    }

    /* End test assert */

    while (1) {
        blink_leds();
        /* Test assert - REMOVE IN FUTURE LABS */
        /*
         * Increase memory allocated for array by 500 chars
         * until we have eaten it all and print space between stack and heap.
         * That is how assert works in run-time.
         */
        array = realloc( array, (i++ * 500) * sizeof(char));
        fprintf(stderr, "%d\n", freeMem());

        if (array == NULL) {
            // Always test if malloc succeeds
            assert(array != NULL); // Fire assert and print error to stderr
            return; // Exit from program anyway when assert is disabled with NDEBUG.
        }

        /* End test assert */
    }
}

