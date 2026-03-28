//
// Created by Denis on 10/27/2025.
//

#include "queue.h"
#include <stdio.h>

#include "../Globals/globals.h"


void queue_init(queue_t* queue) {
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}

int queue_is_empty(const queue_t* queue) {
    return queue->size == 0;
}

void enqueue(queue_t* queue, TCB_t *task) {
    queue->tcb[queue->rear] = task;
    queue->rear = (queue->rear + 1) % MAX_TASKS;
    queue->size++;
#ifdef DEBUG
    print_queue(queue);
#endif
}

TCB_t *dequeue(queue_t* queue) {
    TCB_t *task = peek(queue);

    if(task == internal_idle_task)
        return task;

    queue->front = (queue->front + 1) % MAX_TASKS;
    queue->size--;
#ifdef DEBUG
    print_queue(queue);
    print_task(&task);
#endif

    return task;
}

TCB_t *peek(const queue_t* queue) {
    if(queue_is_empty(queue))
        return internal_idle_task;

    return queue->tcb[queue->front];
}

void print_queue(const queue_t* queue) {
    printf("Printing queue with front : %d and rear : %d: \n", queue->front, queue->rear);
    for(int i = queue->front;i != queue->rear;i = (i + 1) % MAX_TASKS)
        print_task(queue->tcb[i]);

    printf("\n");
}