#include "task_queues.h"
#include <stdio.h>

void task_queues_init(task_queues_t* tq) {
    if (tq == NULL) {
        return;
    }

    // Initialize priority queues
    for (int i = 0; i < NUM_PRIORITY_LEVELS; i++) {
        circular_queue_init(&tq->priority_queues[i]);
    }

    // Initialize pending queue
    circular_queue_init(&tq->pending_queue);
}

bool task_queues_enqueue(task_queues_t* tq, const TCB_t task) {
    if (tq == NULL) {
        printf("Task queues not initialized!\n");
        return false;
    }

    if (task.priority < VERY_HIGH || task.priority > LOW) {
        printf("Invalid priority level!\n");
        return false;
    }

    return circular_enqueue(&tq->priority_queues[task.priority], task);
}

TCB_t task_queues_dequeue(task_queues_t* tq) {
    TCB_t empty_task = { -1, -1 };

    if (tq == NULL) {
        printf("Task queues not initialized!\n");
        return empty_task;
    }

    // Dequeue from highest priority queue first
    for (int i = 0; i < NUM_PRIORITY_LEVELS; i++) {
        if (!circular_queue_is_empty(&tq->priority_queues[i])) {
            return circular_dequeue(&tq->priority_queues[i]);
        }
    }

    printf("All priority queues are empty!\n");
    return empty_task;
}

bool task_queues_move_to_pending(task_queues_t* tq, const TCB_t task) {
    if (tq == NULL) {
        printf("Task queues not initialized!\n");
        return false;
    }

    return circular_enqueue(&tq->pending_queue, task);
}

bool task_queues_restore_from_pending(task_queues_t* tq) {
    if (tq == NULL) {
        printf("Task queues not initialized!\n");
        return false;
    }

    if (circular_queue_is_empty(&tq->pending_queue)) {
        printf("Pending queue is empty!\n");
        return false;
    }

    TCB_t task = circular_dequeue(&tq->pending_queue);
    return task_queues_enqueue(tq, task);
}

void task_queues_print_status(const task_queues_t* tq) {
    if (tq == NULL) {
        printf("Task queues not initialized!\n");
        return;
    }

    const char* priority_names[] = { "VERY_HIGH", "HIGH", "MEDIUM", "LOW" };

    printf("\n=== Task Queues Status ===\n");
    for (int i = 0; i < NUM_PRIORITY_LEVELS; i++) {
        printf("%s queue: %d tasks\n",
            priority_names[i],
            circular_queue_get_size(&tq->priority_queues[i]));
    }
    printf("PENDING queue: %d tasks\n",
        circular_queue_get_size(&tq->pending_queue));
    printf("========================\n\n");
}