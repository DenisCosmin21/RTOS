//
// Created by Denis on 11/23/2025.
//

#include "globals.h"
#include "../RmsScheduler/rms_scheduler.h"

scheduler_t scheduler;
rms_task_templates_t task_templates;
TCB_t *running_task = 0x00;
TCB_t *next_task = 0x00;
int current_time = 0;