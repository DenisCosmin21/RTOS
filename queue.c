//
// Created by Denis on 10/27/2025.
//

#include "queue.h"
#include "task.h"


void queue_init(queue_t* queue) {
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
}

int queue_is_empty(const queue_t* queue) {
    return queue->size == 0;
}

void enqueue(queue_t* queue, const TCB_t task) {
    queue->tcb[queue->rear] = task;
    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    queue->size++;
}

TCB_t dequeue(queue_t* queue) {
    TCB_t task = peek(queue);
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    queue->size--;
    return task;
}

TCB_t peek(const queue_t* queue) {
    if(queue_is_empty(queue))
        return (TCB_t){LOW, -1};

    return queue->tcb[queue->front];
}