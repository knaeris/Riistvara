#include "hmi.h"
#include <avr/pgmspace.h>

const char ver_fw[] PROGMEM =  VER_FW;
const char ver_libc[] PROGMEM = VER_LIBC;
const char stud_name[] PROGMEM = "Kristo Naeris";

const char m1[] PROGMEM = "January";
const char m2[] PROGMEM = "February";
const char m3[] PROGMEM = "March";
const char m4[] PROGMEM = "April";
const char m5[] PROGMEM = "May";
const char m6[] PROGMEM = "June";

PGM_P const name_month[] PROGMEM = {
    m1,
    m2,
    m3,
    m4,
    m5,
    m6
};

const char string_1[] PROGMEM = "                           (o)(o)";
const char string_2[] PROGMEM = "                          /     \\";
const char string_3[] PROGMEM = "                         /       |";
const char string_4[] PROGMEM = "                        /   \\  * |";
const char string_5[] PROGMEM = "          ________     /    /\\__/";
const char string_6[] PROGMEM = "  _      /        \\   /    /";
const char string_7[] PROGMEM = " / \\    /  ____    \\_/    /";
const char string_8[] PROGMEM = "//\\ \\  /  /    \\         /";
const char string_9[] PROGMEM = "V  \\ \\/  /      \\       /";
const char string_10[] PROGMEM = "    \\___/        \\_____/";

PGM_P const banner[] PROGMEM = {
    string_1,
    string_2,
    string_3,
    string_4,
    string_5,
    string_6,
    string_7,
    string_8,
    string_9,
    string_10
};

