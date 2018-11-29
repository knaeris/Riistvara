//TODO There are most likely unnecessary includes. Clean up during lab6
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "../lib/hd44780_111/hd44780.h"
#include "../lib/andygock_avr_uart/uart.h"
#include "../lib/helius_microrl/microrl.h"
#include "hmi.h"
#include "cli_microrl.h"
#include "print_helper.h"

#define NUM_ELEMS(x)    (sizeof(x) / sizeof((x)[0]))

void cli_print_help(const char *const *argv);
void cli_example(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_banner(const char *const *argv);
void cli_handle_month(const char *const *argv);

typedef struct cli_cmd {
    PGM_P cmd;
    PGM_P help;
    void (*func_p)();
    const uint8_t func_argc;
} cli_cmd_t;

const char help_cmd[] PROGMEM = "help";
const char help_help[] PROGMEM = "Get help";
const char ver_cmd[] PROGMEM = "version";
const char ver_help[] PROGMEM = "Print FW version";
const char example_cmd[] PROGMEM = "example";
const char example_help[] PROGMEM =
    "Prints out all provided 3 arguments Usage: example <argument> <argument> <argument>";
const char banner_cmd[] PROGMEM = "banner";
const char banner_help[] PROGMEM = "Print banner";
const char month_cmd[] PROGMEM = "month";
const char month_help[] PROGMEM =
    "Print and display matching months by entered letters from beginning Usage";

const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {example_cmd, example_help, cli_example, 3},
    {banner_cmd, banner_help, cli_print_banner, 0},
    {month_cmd, month_help, cli_handle_month, 1}
};

void cli_print_help(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(PSTR("Implemented commands:\r\n"));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        uart0_puts_p(cli_cmds[i].cmd);
        uart0_puts_p(PSTR(" : "));
        uart0_puts_p(cli_cmds[i].help);
        uart0_puts_p(PSTR("\r\n"));
    }
}

void cli_print_banner(const char *const *argv)
{
    (void) argv;
    print_banner_P(uart0_puts_p, banner, BANNER_ROWS);
}

void cli_handle_month(const char *const *argv)
{
    lcd_clr(LCD_ROW_2_START, LCD_VISIBLE_COLS);
    lcd_goto(LCD_ROW_2_START);

    for (uint8_t i = 0; i < NAME_MONTH_COUNT; i++) {
        if (!strncmp_P(argv[1], (PGM_P) pgm_read_word(&(name_month[i])),
                       (unsigned) (strlen(argv[1])))) {
            uart0_puts_p( (PGM_P) pgm_read_word(&(name_month[i])));
            uart0_puts_p(PSTR("\r\n"));
            lcd_puts_P((PGM_P) pgm_read_word(&(name_month[i])));
            lcd_putc(' ');
        }
    }
}

void cli_example(const char *const *argv)
{
    uart0_puts_p(PSTR("Command had following arguments:\r\n"));

    for (uint8_t i = 1; i < 4; i++) {
        uart0_puts(argv[i]);
        uart0_puts_p(PSTR("\r\n"));
    }
}

void cli_print_ver(const char *const *argv)
{
    (void) argv;
    uart0_puts_p(ver_fw);
    uart0_puts_p(ver_libc);
}

int cli_execute(int argc, const char *const *argv)
{
    // Move cursor to new line. Then user can see what was entered.
    //FIXME Why microrl does not do it?
    uart0_puts_p(PSTR("\r\n"));

    for (uint8_t i = 0; i < NUM_ELEMS(cli_cmds); i++) {
        if (!strcmp_P(argv[0], cli_cmds[i].cmd)) {
            // Test do we have correct arguments to run command
            // Function arguments count shall be defined in struct
            if ((argc - 1) != cli_cmds[i].func_argc) {
                uart0_puts_p(
                    PSTR("To few or too many arguments for this command.\r\n\tUse <help>\r\n"));
                return 1;
            }

            // Hand argv over to function via function pointer,
            // cross fingers and hope that funcion handles it properly
            cli_cmds[i].func_p(argv);
            return 0;
        }
    }

    uart0_puts_p(PSTR("Command not implemented.\r\n\tUse <help> to get help.\r\n"));
    return 1;
}
