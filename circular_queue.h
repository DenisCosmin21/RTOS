
#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_SIZE 10
#include "task.h"
#include <stdbool.h>

typedef struct {
    TCB_t tcb[CIRCULAR_QUEUE_SIZE];
    int front;
    int rear;
    int size;
} circular_queue_t;


void circular_queue_init(circular_queue_t* queue);
bool circular_enqueue(circular_queue_t* queue, const TCB_t task);
TCB_t circular_dequeue(circular_queue_t* queue);
TCB_t circular_peek(const circular_queue_t* queue);
bool circular_queue_is_empty(const circular_queue_t* queue);
bool circular_queue_is_full(const circular_queue_t* queue);
int circular_queue_get_size(const circular_queue_t* queue);

#endif //CIRCULAR_QUEUE_H