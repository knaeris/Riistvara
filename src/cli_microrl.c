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
#include "../lib/matejx_avr_lib/mfrc522.h"
#include "../lib/andy_brown_memdebug/memdebug.h"
#include "../lib/matejx_avr_lib/spi.h"
#include "../lib/matejx_avr_lib/hwdefs.h"
#include "../lib/matejx_avr_lib/hwdefs_minimal.h"

#define NUM_ELEMS(x)    (sizeof(x) / sizeof((x)[0]))

void cli_print_help(const char *const *argv);
void cli_example(const char *const *argv);
void cli_print_ver(const char *const *argv);
void cli_print_banner(const char *const *argv);
void cli_handle_month(const char *const *argv);
void rfid_read_uid(const char *const *argv);
void print_bytes(void (*putc_function)(uint8_t data), const uint8_t *array,
                 const size_t len);
void cli_mem_stat(const char *const *argv);
void cli_rfid_print_list(const char *const *argv);
void cli_rfid_add(const char *const *argv);
void cli_rfid_remove(const char *const *argv);

rfid_card_t *head = NULL;

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
const char rfid_read_uid_cmd[] PROGMEM = "read-uid";
const char rfid_read_uid_help[] PROGMEM = "Read MIFARE card and print Card ID";
const char mem_stat_cmd[] PROGMEM = "mem";
const char mem_stat_help[] PROGMEM =
    "Print memory usage and change compared to previous call";
const char rfid_add_cmd[] PROGMEM = "add";
const char rfid_add_help[] PROGMEM = "Add cards to list: add <UID> <Name>";
const char rfid_list_cmd[] PROGMEM = "print";
const char rfid_list_help[] PROGMEM = "Print list of cards";
const char rfid_remove_cmd[] PROGMEM = "rm";
const char rfid_remove_help[] PROGMEM = "Remove card from list: remove <UID>";

const cli_cmd_t cli_cmds[] = {
    {help_cmd, help_help, cli_print_help, 0},
    {ver_cmd, ver_help, cli_print_ver, 0},
    {example_cmd, example_help, cli_example, 3},
    {banner_cmd, banner_help, cli_print_banner, 0},
    {month_cmd, month_help, cli_handle_month, 1},
    {rfid_read_uid_cmd, rfid_read_uid_help, rfid_read_uid, 0},
    {mem_stat_cmd, mem_stat_help, cli_mem_stat, 0},
    {rfid_add_cmd, rfid_add_help, cli_rfid_add, 2},
    {rfid_list_cmd, rfid_list_help, cli_rfid_print_list, 0},
    {rfid_remove_cmd, rfid_remove_help, cli_rfid_remove, 1}
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

void cli_mem_stat(const char *const *argv)
{
    (void) argv;
    char print_buf[256] = {0x00}; //FIXME reduce size if needed
    uint16_t space = getFreeMemory();
    static uint16_t prev_space;
    uart0_puts_p(PSTR("\r\nSpace between stack and heap:\r\n"));
    sprintf_P(print_buf, PSTR("Current  %d\r\nPrevious %d\r\nChange   %d\r\n"),
              space, prev_space, space - prev_space);
    uart0_puts(print_buf);
    uart0_puts_p(PSTR("\r\nHeap statistics\r\n"));
    sprintf_P(print_buf, PSTR("Used: %u\r\nFree: %u\r\n"), getMemoryUsed(),
              getFreeMemory());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest non freelist block:          %u\r\n"),
              getLargestNonFreeListBlock());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest dynamically allocable block: %u\r\n"),
              getLargestAvailableMemoryBlock());
    uart0_puts(print_buf);
    uart0_puts_p(PSTR("\r\nFreelist\r\n"));
    sprintf_P(print_buf, PSTR("Freelist size:             %u\r\n"),
              getFreeListSize());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Blocks in freelist:        %u\r\n"),
              getNumberOfBlocksInFreeList());
    uart0_puts(print_buf);
    sprintf_P(print_buf, PSTR("Largest block in freelist: %u\r\n"),
              getLargestBlockInFreeList());
    uart0_puts(print_buf);
    prev_space = space; // Document space change for next call
}

static inline uint8_t detect_card(void)
{
    // Not used, but library requires those
    uint8_t bufferATQA[2];
    uint8_t bufferSize = sizeof(bufferATQA);

    if (PICC_IsNewCardPresent()) {
        return 1;
    }

    //Try to wake card
    if (PICC_WakeupA(bufferATQA, &bufferSize) == (STATUS_OK || STATUS_COLLISION)) {
        return 1;
    } else {
        return 0;
    }

    return 0;
}

void print_bytes(void (*putc_function)(uint8_t data), const uint8_t *array,
                 const size_t len)
{
    for (size_t i = 0; i < len; i++) {
        putc_function((array[i] >> 4) + ((array[i] >> 4) <= 9 ? 0x30 : 0x37));
        putc_function((array[i] & 0x0F) + ((array[i] & 0x0F) <= 9 ? 0x30 : 0x37));
    }
}

void rfid_read_uid(const char *const *argv)
{
    (void) argv;
    Uid uid;

    if (!detect_card()) {
        uart0_puts_p(PSTR("Unable to detect card.\r\n"));
        return;
    }

    uart0_puts_p(PSTR("Card selected!\r\n"));
    PICC_ReadCardSerial(&uid);
    uart0_puts_p(PSTR("Card type: "));
    uart0_puts(PICC_GetTypeName(PICC_GetType(uid.sak)));
    uart0_puts_p(PSTR("\r\n"));
    uart0_puts_p(PSTR("Card UID: "));
    print_bytes(uart0_putc, uid.uidByte, uid.size);
    uart0_puts_p(PSTR(" (size "));
    print_bytes(uart0_putc, &uid.size, sizeof(uid.size));
    uart0_puts_p(PSTR(" bytes)\r\n"));
}

void cli_rfid_add(const char *const *argv)
{
    (void) *argv;
    char *uidName;
    uint8_t uidSizeInt;
    char *uidSize;
    char *userName;
    uidName = malloc(strlen(argv[1]) + 1);
    strcpy(uidName, argv[1]);
    uidSizeInt = strlen(uidName) / 2;

    if (uidSizeInt > 10) {
        uart0_puts_p(PSTR("UID size too long!\r\n"));
        free(uidName);
        return;
    }

    rfid_card_t *checkerAdd = head;

    while (checkerAdd != NULL) {
        if (strcmp(checkerAdd->UID, uidName) == 0) {
            uart0_puts_p(PSTR("This card already exist!\r\n"));
            free(uidName);
            return;
        }

        checkerAdd = checkerAdd->next;
    }

    rfid_card_t *newCard = (malloc(sizeof(rfid_card_t)));

    if (newCard == NULL) {
        uart0_puts_p(PSTR("Memory operation failed!\r\n"));
        free(newCard);
        free(uidName);
        return;
    }

    userName = malloc(strlen(argv[2]) + 1);
    strcpy(userName, argv[2]);
    uidSize = malloc(uidSizeInt + 1);
    uidSize = itoa(uidSizeInt, uidSize, 10);
    newCard->UID = uidName;
    newCard->size = uidSize;
    newCard->name = userName;
    newCard->next = NULL;

    if (head == NULL) {
        head = newCard;
        uart0_puts_p(PSTR("Successfully added card!\r\n"));
    } else {
        rfid_card_t *temp = head;

        while (temp->next != NULL) {
            temp = temp->next;
        }

        temp->next = newCard;
        uart0_puts_p(PSTR("Successfully added card!\r\n"));
    }

    uart0_puts_p(PSTR("UID: "));
    uart0_puts(newCard->UID);
    uart0_puts_p(PSTR("\r\nUID size: "));
    uart0_puts(newCard->size);
    uart0_puts_p(PSTR("\r\nName: "));
    uart0_puts(newCard->name);
    uart0_puts_p(PSTR("\r\n"));
}

void cli_rfid_print_list(const char *const *argv)
{
    (void) argv;
    rfid_card_t *current = head;
    int number = 1;
    char* numberChar = NULL;

    if (head == NULL) {
        uart0_puts_p(PSTR("List is empty\r\n"));
    } else {
        while (current != NULL) {
            uart0_puts_p(PSTR("Number: "));
            uart0_puts(itoa(number, numberChar, 10));
            uart0_puts_p(PSTR("\r\nUID size: "));
            uart0_puts(current->size);
            uart0_puts_p(PSTR("\r\nUID: "));
            uart0_puts(current->UID);
            uart0_puts_p(PSTR("\r\nName: "));
            uart0_puts(current->name);
            uart0_puts_p(PSTR("\r\n\r\n"));
            number++;
            current = current->next;
        }
    }
}

void cli_rfid_remove(const char *const *argv)
{
    (void) argv;

    if (head == NULL) {
        uart0_puts_p(PSTR("List is empty\r\n"));
    } else {
        rfid_card_t *current = head;
        rfid_card_t *previous = head;
        rfid_card_t *temp_card = NULL;
        char *uidName = malloc(strlen(argv[1]) + 1);
        strcpy(uidName, argv[1]);

        if (strcmp(current->UID, uidName) == 0) {
            temp_card = head->next;
            free(head);
            free(uidName);
            head = temp_card;
            uart0_puts_p(PSTR("Card was removed!\r\n"));
            return;
        }

        while (current != NULL) {
            if (strcmp(current->UID, uidName) == 0) {
                rfid_card_t *temp_card = current;
                previous->next = current->next;
                free(temp_card);
                free(uidName);
                uart0_puts_p(PSTR("Card was removed successfully!\r\n"));
                return;
            }

            previous = current;
            current = current->next;
        }

        free(uidName);
        uart0_puts_p(PSTR("This card doesn't exist!\r\n"));
    }
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
