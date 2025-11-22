//
// Created by Denis on 11/22/2025.
//

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#define QUEUE_SIZE 10
#include "task.h"

typedef struct {
    TCB_t tcb[QUEUE_SIZE];
    int front;
    int rear;
} linear_priority_queue_t;

void p_init(linear_priority_queue_t *);

void p_enqueue(linear_priority_queue_t *,const TCB_t);

TCB_t p_dequeue(linear_priority_queue_t *);

TCB_t p_peek(const linear_priority_queue_t *);

int p_get_size(const linear_priority_queue_t *);

#endif //PRIORITY_QUEUE_H
