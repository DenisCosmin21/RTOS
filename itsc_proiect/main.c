#include <stdio.h>
#include "task_queues.h"

int main(void) {
    printf("=== Task Queue System Demo ===\n\n");

    // Initialize task queue system (static allocation)
    task_queues_t tq;
    task_queues_init(&tq);

    // Create and enqueue tasks with different priorities
    printf("Enqueuing tasks...\n");
    task_queues_enqueue(&tq, newTask(LOW, 100));
    task_queues_enqueue(&tq, newTask(VERY_HIGH, 101));
    task_queues_enqueue(&tq, newTask(MEDIUM, 102));
    task_queues_enqueue(&tq, newTask(HIGH, 103));
    task_queues_enqueue(&tq, newTask(LOW, 104));
    task_queues_enqueue(&tq, newTask(VERY_HIGH, 105));

    task_queues_print_status(&tq);

    // Dequeue tasks (should come out in priority order)
    printf("Dequeuing tasks by priority:\n");
    TCB_t task1 = task_queues_dequeue(&tq);
    printf("Dequeued: Priority=%d, Value=%d\n", task1.priority, task1.value);

    TCB_t task2 = task_queues_dequeue(&tq);
    printf("Dequeued: Priority=%d, Value=%d\n", task2.priority, task2.value);

    task_queues_print_status(&tq);

    // Move a task to pending queue
    printf("Moving task to pending queue...\n");
    task_queues_move_to_pending(&tq, newTask(HIGH, 200));

    task_queues_print_status(&tq);

    // Restore task from pending queue
    printf("Restoring task from pending queue...\n");
    task_queues_restore_from_pending(&tq);

    task_queues_print_status(&tq);

    // Dequeue remaining tasks
    printf("Dequeuing all remaining tasks:\n");
    TCB_t task;
    while (1) {
        task = task_queues_dequeue(&tq);
        if (task.priority == -1) {
            break;
        }
        printf("Dequeued: Priority=%d, Value=%d\n", task.priority, task.value);
    }

    task_queues_print_status(&tq);

    return 0;
}