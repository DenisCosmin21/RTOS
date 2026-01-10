#ifndef GLOBALS_H
#define GLOBALS_H
#include "task.h"
#include "rms_scheduler.h"
#include <stdint.h>

extern TCB_t *running_task;
extern TCB_t *next_task;
extern uint64_t current_time;
extern TCB_t *internal_idle_task;
extern short started;
extern uint32_t QUANTA;
extern uint32_t Runtime_Task_Profiler;

#endif //GLOBALS_H
