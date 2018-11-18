#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "../lib/eriks_freemem/freemem.h"
#include <avr/pgmspace.h>
#include "uart.h"
#include "hmi.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"
#include <string.h>

#define BLINK_DELAY_MS 100

static inline void init_leds(void)
{
    DDRA |= _BV(PA1);
    DDRA |= _BV(PA3);
    DDRA |= _BV(PA5);
    DDRB |= _BV(DDB7);
    PORTB &= ~_BV(PORTB7);
}

static inline void init_lcd(void)
{
    lcd_init();
    lcd_goto(LCD_ROW_1_START);
    lcd_puts_P(stud_name);
}

static inline void init_errcon(void)
{
    simple_uart0_init();
    simple_uart1_init();
    stdin = stdout = &simple_uart0_io;
    stderr = &simple_uart1_out;
    fprintf_P(stderr, PSTR("Version: " FW_VERSION " built on: " __DATE__
                           " "__TIME__"\n"));
    fprintf_P(stderr, PSTR("avr-libc version: " __AVR_LIBC_VERSION_STRING__
                           " ""avr-gcc version: " __VERSION__ "\n"));
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

static inline void lab4_month_lookup_P(void)
{
    char in_buf = 0;
    printf_P(PSTR("Enter Month name first letter >"));
    scanf("%c", &in_buf);
    printf("%c\n", in_buf);
    lcd_clr(LCD_ROW_2_START, LCD_VISIBLE_COLS);
    lcd_goto(LCD_ROW_2_START);

    for (uint8_t i = 0; i < NAME_MONTH_COUNT; i++) {
        if (!strncmp_P(&in_buf, (PGM_P) pgm_read_word(&(name_month[i])), 1)) {
            fprintf_P(stdout, (PGM_P) pgm_read_word(&(name_month[i])));
            fputc('\n', stdout);
            lcd_puts_P((PGM_P) pgm_read_word(&(name_month[i])));
            lcd_putc(' ');
        }
    }
}

static inline void init_console(void)
{
    fprintf_P(stdout, PSTR("\nConsole Started \n"));
    fprintf_P(stdout, stud_name);
    fprintf(stdout, "\n");
    print_banner_P(stdout, banner, BANNER_ROWS);
}

void main(void)
{
    init_leds();
    init_errcon();
    init_lcd();
    init_console();

    while (1) {
        blink_leds();
        lab4_month_lookup_P();
    }
}

