#include "../lib/andygock_avr_uart/uart.h"
#include "print_helper.h"
#include <avr/pgmspace.h>


void print_banner_P(void (*puts_P_function)(const char*),
                    const char * const *banner, const uint8_t rows)
{
    for (uint8_t row = 0; row < rows; row++) {
        puts_P_function((PGM_P) pgm_read_word(&(banner[row])));
        puts_P_function(PSTR("\r\n"));
    }

    puts_P_function(PSTR("\r\n"));
}
