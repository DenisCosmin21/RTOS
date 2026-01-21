#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include "stm32u5xx.h"



void seven_seg_init(void);
void seven_seg_display_digit(int digit, int number);
void seven_seg_clear(void);

#endif
