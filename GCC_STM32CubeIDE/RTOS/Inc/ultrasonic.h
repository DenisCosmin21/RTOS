#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include "stm32u5xx.h"

// Trig: D2
// Echo: D4
#define ULTRASONIC_TRIG_PIN (15)
#define ULTRASONIC_ECHO_PIN (14)
#define ULTRASONIC_PORT GPIOF

void ultrasonic_init(void);
float ultrasonic_read(void);

#endif
