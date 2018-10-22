/*
 * Developed by Eriks Ocakovskis in 2018 spring semester
 */
#ifndef FREEMEM_H
#define FREEMEM_H
#include <stdint.h> // uint16_t definition

/* Returns free space between stack and heap */
uint16_t freeMem(void);
#endif /* FREEMEM_H */