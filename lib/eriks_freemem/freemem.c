/*
 * Developed by Eriks Ocakovskis in 2018 spring semester
 *
 * Code from http://forum.arduino.cc/index.php?topic=42066.0
 * Interesting pice of code that is based on
 * https://github.com/vancegroup-mirrors/avr-libc/blob/master/avr-libc/libc/stdlib/malloc.c
 */
#include <stdlib.h>
#include <avr/io.h>
#include "freemem.h"

extern char * const __brkval;

uint16_t freeMem(void)
{
    char *brkval;
    char *cp;
    brkval = __brkval;

    if (brkval == 0) {
        brkval = __malloc_heap_start;
    }

    cp = __malloc_heap_end;

    if (cp == 0) {
        cp = ((char *)AVR_STACK_POINTER_REG) - __malloc_margin;
    }

    if (cp <= brkval) {
        return 0;
    }

    return cp - brkval;
}