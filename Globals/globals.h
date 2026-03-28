#ifndef GLOBALS_H
#define GLOBALS_H
#include "../Task/task.h"

extern TCB_t *running_task;
extern TCB_t *next_task;
extern int current_time;
extern short started;
extern TCB_t *internal_idle_task;

#endif //GLOBALS_H
