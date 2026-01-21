#ifndef SERVO_H
#define SERVO_H

#include "stm32u5xx.h"

void servo_init(void);

void servo_set_angle(int angle);

#endif
