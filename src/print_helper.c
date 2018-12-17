#include "../lib/andygock_avr_uart/uart.h"
#include "print_helper.h"
#include <avr/pgmspace.h>
#include "stdlib.h"

void print_banner_P(void (*puts_P_function)(const char*),
                    const char * const *banner, const uint8_t rows)
{
    for (uint8_t row = 0; row < rows; row++) {
        puts_P_function((PGM_P) pgm_read_word(&(banner[row])));
        puts_P_function(PSTR("\r\n"));
    }

    puts_P_function(PSTR("\r\n"));
}

char *bin2hex(unsigned char *p, int len)
{
    char *hex = malloc(((2 * len) + 1));
    char *r = hex;

    while (len && p) {
        (*r) = ((*p) & 0xF0) >> 4;
        (*r) = ((*r) <= 9 ? '0' + (*r) : 'A' - 10 + (*r));
        r++;
        (*r) = ((*p) & 0x0F);
        (*r) = ((*r) <= 9 ? '0' + (*r) : 'A' - 10 + (*r));
        r++;
        p++;
        len--;
    }

    *r = '\0';
    return hex;
}
