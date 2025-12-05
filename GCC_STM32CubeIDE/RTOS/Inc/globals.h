#ifndef GLOBALS_H
#define GLOBALS_H
#include "task.h"
#include "rms_scheduler.h"
#include <stdint.h>

extern TCB_t *running_task;
extern TCB_t *next_task;
extern uint64_t current_time;
#endif //GLOBALS_H
