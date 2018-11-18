#include <avr/pgmspace.h>
#ifndef _HMI_H_
#define _HMI_H_
#define NAME_MONTH_COUNT 6
#define BANNER_ROWS 10

extern const char stud_name[] PROGMEM;
extern PGM_P const name_month[] PROGMEM;
extern PGM_P const banner[] PROGMEM;

#endif /* _HMI_H_ */

