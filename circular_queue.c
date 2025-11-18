#include "circular_queue.h"
#include <stdio.h>

void circular_queue_init(circular_queue_t* queue) {
    if (queue != NULL) {
        queue->front = 0;
        queue->rear = -1;
        queue->size = 0;
    }
}

bool circular_queue_is_empty(const circular_queue_t* queue) {
    return queue == NULL || queue->size == 0;
}

bool circular_queue_is_full(const circular_queue_t* queue) {
    return queue == NULL || queue->size == CIRCULAR_QUEUE_SIZE;
}

int circular_queue_get_size(const circular_queue_t* queue) {
    return queue == NULL ? 0 : queue->size;
}

bool circular_enqueue(circular_queue_t* queue, const TCB_t task) {
    if (circular_queue_is_full(queue)) {
        printf("Circular queue is full! Cannot enqueue.\n");
        return false;
    }

    queue->rear = (queue->rear + 1) % CIRCULAR_QUEUE_SIZE;
    queue->tcb[queue->rear] = task;
    queue->size++;
    return true;
}

TCB_t circular_dequeue(circular_queue_t* queue) {
    TCB_t empty_task = createEmptyTask();

    if (circular_queue_is_empty(queue)) {
        printf("Circular queue is empty! Cannot dequeue.\n");
        return empty_task;
    }

    TCB_t task = queue->tcb[queue->front];
    queue->front = (queue->front + 1) % CIRCULAR_QUEUE_SIZE;
    queue->size--;
    return task;
}

TCB_t circular_peek(const circular_queue_t* queue) {
    TCB_t empty_task = createEmptyTask();

    if (circular_queue_is_empty(queue)) {
        printf("Circular queue is empty! Cannot peek.\n");
        return empty_task;
    }

    return queue->tcb[queue->front];
}