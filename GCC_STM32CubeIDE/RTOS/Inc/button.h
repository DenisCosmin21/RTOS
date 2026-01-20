#ifndef BUTTON_H
#define BUTTON_H

#include "stm32u5xx.h"

#define BUTTON_PIN (13)
#define BUTTON_PORT GPIOC

void button_init(void);
int button_read(void);
void button_exti_init(void);

#endif
