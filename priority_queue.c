//
// Created by Denis on 11/22/2025.
//
#include "priority_queue.h"
#include <stddef.h>
#include <stdio.h>

void p_init(linear_priority_queue_t *queue) {
    queue->rear = 0;
    queue->front = 0;
}

static void shift_right(linear_priority_queue_t *queue, size_t from) {
    for(;from < queue->rear; from++)
        queue->tcb[from + 1] = queue->tcb[from];
}

void p_enqueue(linear_priority_queue_t *queue, TCB_t task, int (*condition)(const TCB_t *, const TCB_t *)) {
    if(is_empty_task(&task))
        return;

    size_t i = 0;

    for(;i < queue->rear;i++) {
        if(!condition(&queue->tcb[i], &task)) {
            shift_right(queue, i);
            break;
        }
    }

    queue->tcb[i] = task;
    queue->rear++;

#ifdef DEBUG
    p_print_queue(queue);
#endif
}

TCB_t p_dequeue(linear_priority_queue_t *queue, int (*condition)(const TCB_t *)) {
    TCB_t task = p_peek(queue, condition);

    if(is_empty_task(&task))
        return task;

    queue->front++;

#ifdef DEBUG
    p_print_queue(queue);
    print_task(&task);
#endif

    return task;
}

TCB_t p_peek(const linear_priority_queue_t *queue, int (*condition)(const TCB_t *)) {
    if(queue->rear == 0 || (queue->front == queue->rear))
        return empty_task();

    if(condition(&queue->tcb[queue->front]))
        return queue->tcb[queue->front];

    return empty_task();
}

int p_get_size(const linear_priority_queue_t *queue) {
    return queue->rear - queue->front;
}

short p_is_empty(const linear_priority_queue_t *queue) {
    return queue->rear == queue->front;
}

void p_print_queue(const linear_priority_queue_t *queue) {
    printf("Printing priority queue with front : %d and rear : %d: \n", queue->front, queue->rear);
    for(size_t i = queue->front; i < queue->rear; i++)
        print_task(&queue->tcb[i]);
    printf("\n");
}
