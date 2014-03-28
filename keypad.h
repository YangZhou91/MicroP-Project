#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f4xx.h"

void keypad_init(void);
uint8_t readKeys(void);
#endif
