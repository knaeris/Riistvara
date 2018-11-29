
#include <avr/io.h>
#include <util/delay.h>
#include "../lib/eriks_freemem/freemem.h"
#include <avr/pgmspace.h>
#include "hmi.h"
#include "print_helper.h"
#include "../lib/hd44780_111/hd44780.h"
#include <string.h>
#include <time.h>
#include "../lib/andygock_avr_uart/uart.h"
#include <avr/interrupt.h>
#include "cli_microrl.h"
#include "../lib/helius_microrl/microrl.h"

#define LED_RED         PORTA1 // Arduino Mega digital pin 23
#define LED_GREEN       PORTA3 // Arduino Mega digital pin 25
#define LED_BLUE        PORTA5 // Arduino Mega digital pin 27
#define UART_BAUD           9600
#define UART_STATUS_MASK    0x00FF
#define BLINK_DELAY_MS 100

static microrl_t rl;
static microrl_t *prl = &rl;

static inline void init_leds(void)
{
    DDRB |= _BV(DDB7);
    PORTB &= ~(PORTB7);
    /* RGB LED board set up and off */
    DDRA |= _BV(LED_RED) | _BV(LED_GREEN) | _BV(LED_BLUE);
    PORTA &= ~(_BV(LED_RED) | _BV(LED_GREEN) | _BV(LED_BLUE));
}

static inline void init_con_uart0(void)
{
    uart0_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    uart0_puts_p(PSTR("Console started\r\n"));
}

static inline void init_sys_timer(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1B |= _BV(WGM12); // Turn on CTC (Clear Timer on Compare)
    TCCR1B |= _BV(CS12); // fCPU/256
    OCR1A = 62549; // Note that it is actually two registers OCR5AH and OCR5AL
    TIMSK1 |= _BV(OCIE1A); // Output Compare A Match Interrupt Enable
}

static inline void blocking_delay(void)
{
    PORTA ^= _BV(LED_RED);
    _delay_ms(BLINK_DELAY_MS);
}

static inline void heartbeat(void)
{
    static time_t prev_time;
    char ascii_buf[11] = {0x00};
    time_t now = time(NULL);

    if (prev_time != now) {
        //Print uptime to uart1
        ltoa(now, ascii_buf, 10);
        uart1_puts_p(PSTR("Uptime: "));
        uart1_puts(ascii_buf);
        uart1_puts_p(PSTR(" s.\r\n"));
        //Toggle LED
        PORTA ^= _BV(LED_GREEN);
        prev_time = now;
    }
}

static inline void init_lcd(void)
{
    lcd_init();
    lcd_goto(LCD_ROW_1_START);
    lcd_puts_P(stud_name);
}

static inline void init_console(void)
{
    uart0_puts_p(stud_name);
    uart0_puts_p(PSTR("\r\n"));
    uart0_puts_p(PSTR("Use backspace to delete entry and enter to confirm.\r\n"));
    uart0_puts_p(PSTR("Arrow key's and del do not work currently.\r\n"));
    microrl_init(prl, uart0_puts);
    microrl_set_execute_callback(prl, cli_execute);
}

static inline void init_ver(void)
{
    uart1_puts_p(ver_fw);
    uart1_puts_p(ver_libc);
}

static inline void init_con_uart1(void)
{
    uart1_init(UART_BAUD_SELECT(UART_BAUD, F_CPU));
    init_ver();
}

void main(void)
{
    init_leds();
    init_lcd();
    init_con_uart0();
    init_console();
    init_con_uart1();
    init_sys_timer();
    sei(); // Enable all interrupts. Needed for UART!!

    while (1) {
        heartbeat();
        microrl_insert_char(prl, (uart0_getc() & UART_STATUS_MASK));
    }
}

ISR(TIMER1_COMPA_vect)
{
    system_tick();
}

