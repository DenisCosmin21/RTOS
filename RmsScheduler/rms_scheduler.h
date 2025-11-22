#ifndef RMS_SCHEDULER_H
#define RMS_SCHEDULER_H

#include "../Task/task.h"
#include "../Queue/binary_heap_queue.h"

typedef struct {
    heap_priority_queue_t tasks;
} rms_task_templates_t;

void rms_task_templates_init(rms_task_templates_t* templates);
short rms_task_templates_add(rms_task_templates_t* templates,const TCB_t task);
short is_schedulable(const rms_task_templates_t* templates);
short start_scheduler(rms_task_templates_t* templates);
TCB_t get_next_task(void);
void wait_task(const TCB_t task);
void release_tasks(void);
short should_preempt(const TCB_t *task);
void store_back_task(const TCB_t *task);
/*
void rms_task_templates_print(const rms_task_templates_t* templates);
void rms_print_analysis(const rms_task_templates_t* templates);
*/
#endif
