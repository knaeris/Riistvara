#ifndef _PRINT_HELPER_H_
#define _PRINT_HELPER_H_

void print_banner_P(void (*puts_P_function)(const char*),const char * const *banner,const uint8_t rows);
char *bin2hex(unsigned char *p, int len);
#endif /* _PRINT_HELPER_H_ */

