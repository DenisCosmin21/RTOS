#ifndef TASK_QUEUES_H
#define TASK_QUEUES_H
#include "circular_queue.h"
#include "task.h"
#include <stdbool.h>

#define NUM_PRIORITY_LEVELS 4

typedef struct {
    circular_queue_t priority_queues[NUM_PRIORITY_LEVELS];
    circular_queue_t pending_queue;
} task_queues_t;

void task_queues_init(task_queues_t* tq);
bool task_queues_enqueue(task_queues_t* tq, const TCB_t task);
TCB_t task_queues_dequeue(task_queues_t* tq);
bool task_queues_move_to_pending(task_queues_t* tq, const TCB_t task);
bool task_queues_restore_from_pending(task_queues_t* tq);
void task_queues_print_status(const task_queues_t* tq);

#endif //TASK_QUEUES_H
