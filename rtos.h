//
// Created by Denis on 11/22/2025.
//

#ifndef RTOS_H
#define RTOS_H

extern int current_time;

short rtos_task_create(const int execution_time,const int period, const char*);

short rtos_start(void);

void simulate_rtos(void);
#endif //RTOS_H
