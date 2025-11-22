#ifndef TASK_QUEUES_H
#define TASK_QUEUES_H
#include "queue.h"
#include "priority_queue.h"
#include "../Task/task.h"
#define NUM_PRIORITY_LEVELS 4

typedef struct {
    queue_t queues[NUM_PRIORITY_LEVELS];
    linear_priority_queue_t pending;
} task_queues_t;

void task_queues_init(task_queues_t* tq);
short task_queues_enqueue(task_queues_t* tq, const TCB_t task);
TCB_t task_queues_dequeue(task_queues_t* tq);
short task_queues_move_to_pending(task_queues_t* tq, const TCB_t task);
short task_queues_restore_from_pending(task_queues_t* tq);
void print_task_queues(task_queues_t* tq);
short exists_higher_priority_task(const task_queues_t* tq, const TCB_t *task);
#endif //TASK_QUEUES_H
