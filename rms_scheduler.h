#ifndef RMS_SCHEDULER_H
#define RMS_SCHEDULER_H

#include "task.h"
#include <stdbool.h>

#define MAX_TASKS 10

typedef struct {
    TCB_t task_templates[MAX_TASKS];
    int num_tasks;
} rms_task_templates_t;

void rms_task_templates_init(rms_task_templates_t* templates);
bool rms_task_templates_add(rms_task_templates_t* templates, TCB_t task);
double calculate_utilization(const rms_task_templates_t* templates);
double calculate_rms_bound(int n);
bool is_schedulable(const rms_task_templates_t* templates);
void rms_task_templates_print(const rms_task_templates_t* templates);
void rms_print_analysis(const rms_task_templates_t* templates);

#endif
