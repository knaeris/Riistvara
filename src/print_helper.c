#include <stdio.h>
#include "print_helper.h"
#include <avr/pgmspace.h>


void print_banner_P(FILE *stream, const char * const *banner,
                    const uint8_t rows)
{
    for (uint8_t row = 0; row < rows; row++) {
        fprintf_P(stream, (PGM_P) pgm_read_word(&(banner[row])));
        fprintf(stream, "\n");
    }

    fprintf(stream, "\n");
}
