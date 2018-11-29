#include <avr/pgmspace.h>
#ifndef _HMI_H_
#define _HMI_H_
#define NAME_MONTH_COUNT 6
#define BANNER_ROWS 10

#define VER_FW      "Version: " FW_VERSION " built on: " __DATE__ " "__TIME__"\r\n"
#define VER_LIBC    "avr-libc version: " __AVR_LIBC_VERSION_STRING__ "avr-gcc version: " __VERSION__ "\r\n"
extern const char stud_name[] PROGMEM;
extern PGM_P const name_month[] PROGMEM;
extern PGM_P const banner[] PROGMEM;
extern const char ver_fw[] PROGMEM;
extern const char ver_libc[] PROGMEM;
#endif /* _HMI_H_ */

