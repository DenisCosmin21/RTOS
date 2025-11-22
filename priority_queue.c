//
// Created by Denis on 11/22/2025.
//
#include "priority_queue.h"
#include <stddef.h>

void p_init(linear_priority_queue_t *queue, const TCB_t task) {
    queue->rear = 0;
    queue->front = 0;
}

static void shift_right(linear_priority_queue_t *queue, size_t from) {
    for(;from < queue->rear; from++)
        queue->tcb[from + 1] = queue->tcb[from];
}

void p_enqueue(linear_priority_queue_t *queue, const TCB_t task) {
    size_t i = 0;

    for(;i < queue->rear;i++) {
        if(queue->tcb[i].priority > task.priority) {
            shift_right(queue, i);
        }
    }

    queue->tcb[i] = task;
    queue->rear++;
}

TCB_t p_dequeue(linear_priority_queue_t *queue) {
    TCB_t task = p_peek(queue);

    if(is_empty_task(&task))
        return task;

    queue->front++;

    return task;
}

TCB_t p_peek(const linear_priority_queue_t *queue) {
    if(queue->rear == 0)
        return empty_task();

    return queue->tcb[queue->front];
}

int p_get_size(const linear_priority_queue_t *queue) {
    return queue->rear - queue->front;
}
