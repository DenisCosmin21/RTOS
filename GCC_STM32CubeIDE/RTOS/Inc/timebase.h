#ifndef __TIMEBASE_H__
#define __TIMEBASE_H__
#include <stdint.h>

uint32_t get_tick(void);
uint32_t get_time_us(void);
void timebase_init(void);

void delay(uint32_t delay);


#endif
