//
// Created by Denis on 11/22/2025.
//
#ifndef BINARY_HEAP_QUEUE_H
#define BINARY_HEAP_QUEUE_H
#define MAX_TASKS 10
#include <stddef.h>
#include "task.h"
#include "rtos_config.h"

typedef struct {
    TCB_t *tcb[MAX_TASKS];
    size_t size;
}heap_priority_queue_t;

void h_init(heap_priority_queue_t *);

void h_enqueue(heap_priority_queue_t *,TCB_t *, int (*condition)(const TCB_t *, const TCB_t *));

TCB_t *h_dequeue(heap_priority_queue_t *, int (*pop_condition)(const TCB_t *), int (*priority_condition)(const TCB_t *, const TCB_t *));

TCB_t *h_peek(const heap_priority_queue_t *, int (*pop_condition)(const TCB_t *));

size_t h_get_size(const heap_priority_queue_t *);

short h_is_empty(const heap_priority_queue_t *);

void h_print_queue(const heap_priority_queue_t *);
#endif //BINARY_HEAP_QUEUE_H
