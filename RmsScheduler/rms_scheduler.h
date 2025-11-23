#ifndef RMS_SCHEDULER_H
#define RMS_SCHEDULER_H
#include "../Task/task.h"
#include "../Queue/binary_heap_queue.h"
#include "../Queue/queue.h"

#define NUM_PRIORITY_LEVELS 4
#define MAX_PRIORITY_COUNT sizeof(long) * 8

typedef struct {
    heap_priority_queue_t tasks;
} rms_task_templates_t;

typedef struct {
    queue_t queues[NUM_PRIORITY_LEVELS];
    heap_priority_queue_t pending;
    unsigned long bitmap; //Bitmap to specify if there is a task on a specific priority. bit for speicif cpriority 1 if exists else 0
} scheduler_t;

void scheduler_init(scheduler_t* scheduler, rms_task_templates_t* templates);

short rms_task_templates_add(rms_task_templates_t* templates,TCB_t *task);

short is_schedulable(const rms_task_templates_t* templates);

short start_scheduler(rms_task_templates_t* templates, scheduler_t *scheduler);

TCB_t *scheduler_get_task(scheduler_t*);

short scheduler_add_task(scheduler_t*, TCB_t *task);

short scheduler_sleep_task(scheduler_t*, TCB_t *task);

short exists_higher_priority_task(const scheduler_t*, const TCB_t *task);

short scheduler_release_tasks(scheduler_t*);

void print_scheduler(const scheduler_t* scheduler);

#endif
