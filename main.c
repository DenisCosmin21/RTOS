#include <stdio.h>
#include "queue.h"
#include "task.h"

int main(void) {
    TCB_t task1 = newTask(LOW, 2);
    TCB_t task2 = newTask(HIGH, 2);
    TCB_t task3 = newTask(LOW, 3);
    TCB_t task;
    queue_t queue;
    queue_init(&queue);
    task = peek(&queue);
    enqueue(&queue, task1);
    enqueue(&queue, task2);
    task = peek(&queue);
    task = dequeue(&queue);
    enqueue(&queue, task3);
    task = dequeue(&queue);
    task = dequeue(&queue);
}
