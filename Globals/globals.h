#ifndef GLOBALS_H
#define GLOBALS_H
#include "../Task/task.h"
#include "../RmsScheduler/rms_scheduler.h"

extern scheduler_t scheduler;
extern rms_task_templates_t rms_task_templates;
extern TCB_t *running_task;
extern TCB_t *next_task;
extern int current_time;
#endif //GLOBALS_H
