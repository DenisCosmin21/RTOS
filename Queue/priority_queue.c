//
// Created by Denis on 11/22/2025.
//
#include "priority_queue.h"
#include <stdio.h>

void p_init(linear_priority_queue_t *queue) {
    queue->rear = 0;
}

static void shift_right(linear_priority_queue_t *queue, size_t from) {
    for(size_t i = queue->rear; i  > from; i--)
        queue->tcb[i] = queue->tcb[i - 1];
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

static void shift_left(linear_priority_queue_t *queue) {
    for(size_t i = 0;i < queue->rear - 1;i++)
        queue->tcb[i] = queue->tcb[i + 1];
}

TCB_t p_dequeue(linear_priority_queue_t *queue, int (*condition)(const TCB_t *)) {
    TCB_t task = p_peek(queue, condition);

    if(is_empty_task(&task))
        return task;

    shift_left(queue);
    queue->rear--;
#ifdef DEBUG
    p_print_queue(queue);
    print_task(&task);
#endif

    return task;
}

TCB_t p_peek(const linear_priority_queue_t *queue, int (*condition)(const TCB_t *)) {
    if(queue->rear == 0)
        return empty_task();

    if(condition(&queue->tcb[0]))
        return queue->tcb[0];

    return empty_task();
}

int p_get_size(const linear_priority_queue_t *queue) {
    return queue->rear;
}

short p_is_empty(const linear_priority_queue_t *queue) {
    return queue->rear == 0;
}

void p_print_queue(const linear_priority_queue_t *queue) {
    printf("Printing priority queue with rear : %d: \n", queue->rear);
    for(size_t i = 0; i < queue->rear; i++)
        print_task(&queue->tcb[i]);
    printf("\n");
}
