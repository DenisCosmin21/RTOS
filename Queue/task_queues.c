#include "task_queues.h"
#include <stdio.h>
#include "../RtosApi/rtos.h"

static int pending_enqueue(const TCB_t *task1, const TCB_t *task2) {
    return task1->next_release_time > task2->next_release_time;
}

static int should_restore(const TCB_t *task) {
    return task->next_release_time -1 == current_time;
}

void task_queues_init(task_queues_t* tq) {
    for (int i = 0; i < NUM_PRIORITY_LEVELS; i++) {
        queue_init(&tq->queues[i]);
    }

    h_init(&tq->pending);
}

short task_queues_enqueue(task_queues_t* tq, TCB_t *task) {
    if (task->priority < VERY_HIGH || task->priority > LOW) {
        return 0;
    }

    enqueue(&tq->queues[task->priority], task);

#ifdef DEBUG
    print_task_queues(tq);
#endif
    return 1;
}

TCB_t *task_queues_dequeue(task_queues_t* tq) {
    TCB_t *task;

    for (int i = 0; i < NUM_PRIORITY_LEVELS; i++) {
        task = dequeue(&tq->queues[i]);
        if(task != 0x00) {
            #ifdef DEBUG
                print_task_queues(tq);
            #endif
            return task;
        }
    }

    return 0x00;
}

short task_queues_move_to_pending(task_queues_t* tq, TCB_t *task) {
    h_enqueue(&tq->pending, task, pending_enqueue);
#ifdef DEBUG
    print_task_queues(tq);
#endif

    return 1;
}

short task_queues_restore_from_pending(task_queues_t* tq) {
    TCB_t *pending_task = h_dequeue(&tq->pending, should_restore, pending_enqueue);

    if(pending_task == 0x00)
        return 0;

    pending_task->remaining_time = pending_task->execution_time;
    task_queues_enqueue(tq, pending_task);

#ifdef DEBUG
    print_task_queues(tq);
    print_task(&pending_task);
#endif

    return 1;
}

short exists_higher_priority_task(const task_queues_t* tq, const TCB_t *task) {
    for(size_t priority = 0; priority < task->priority && priority < NUM_PRIORITY_LEVELS; priority++) {
        TCB_t *priority_task = peek(&tq->queues[priority]);
        if(priority_task != 0x00)
            return 1;
    }

    return 0;
}

void print_task_queues(task_queues_t* tq) {
    printf("Printing task queues: \n");
    for (int i = 0; i < NUM_PRIORITY_LEVELS; i++) {
        switch(i) {
            case 0: {
                printf("Very high : ");
                break;
            }
            case 1: {
                printf("High : ");
                break;
            }
            case 2: {
                printf("Medium : ");
                break;
            }
            case 3: {
                printf("Low : ");
                break;
            }
            default: {
                printf("Unknown priority : %d\n", i);
            }
        }
        print_queue(&tq->queues[i]);
    }


    printf("Printing pending queue: \n");
    h_print_queue(&tq->pending);
}