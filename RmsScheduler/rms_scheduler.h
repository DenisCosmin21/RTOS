#ifndef RMS_SCHEDULER_H
#define RMS_SCHEDULER_H

#define NUM_PRIORITY_LEVELS 4

#include "../rtos_config.h"
#include "../Task/task.h"
#include "../Queue/binary_heap_queue.h"
#include "../Queue/queue.h"

#define MAX_PRIORITY_COUNT 32

#if NUM_PRIORITY_LEVELS > MAX_PRIORITY_COUNT
#undef NUM_PRIORITY_LEVELS
#define NUM_PRIORITY_LEVELS MAX_PRIORITY_COUNT
#endif

typedef struct {
    heap_priority_queue_t tasks;
} rms_task_templates_t;

typedef struct {
    queue_t queues[NUM_PRIORITY_LEVELS];
    heap_priority_queue_t pending;
    unsigned long bitmap; //Bitmap to specify if there is a task on a specific priority. bit for speicif cpriority 1 if exists else 0
} scheduler_t;

void scheduler_init(void);

short rms_task_templates_add(TCB_t *task);

short is_schedulable(void);

short start_scheduler(void);

TCB_t *scheduler_get_task(void);

short scheduler_add_task(TCB_t *task);

short scheduler_sleep_task(TCB_t *task);

short exists_higher_priority_task(const TCB_t *task);

void scheduler_release_tasks(void);

short exists_any_task(void);

void print_scheduler(void);

void prepare_next_task(TCB_t *);

void add_next_task();
#endif
