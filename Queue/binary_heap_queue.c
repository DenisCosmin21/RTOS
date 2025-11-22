//
// Created by Denis on 11/22/2025.
//

#include "binary_heap_queue.h"

#include <stdio.h>

void h_init(heap_priority_queue_t * queue) {
    queue->size = 0;
}

static size_t parent(const size_t position) {
    return (position - 1) / 2;
}

static size_t left(const size_t position) {
    return position * 2 + 1;
}

static size_t right(const size_t position) {
    return position * 2 + 2;
}

static void swap_elements(TCB_t **task1, TCB_t **task2) {
    TCB_t *tmp = *task1;
    *task1 = *task2;
    *task2 = tmp;
}

static void shift_heap(heap_priority_queue_t * queue, size_t position, int (*condition)(const TCB_t *, const TCB_t *)) {
    while(position != 0 && condition(queue->tcb[parent(position)], queue->tcb[position])) {
        swap_elements(&queue->tcb[parent(position)], &queue->tcb[position]);
    }
}

void h_enqueue(heap_priority_queue_t * queue,TCB_t *task, int (*priority_condition)(const TCB_t *, const TCB_t *)) {
    queue->size++;
    size_t position = queue->size - 1;
    queue->tcb[position] = task;

    shift_heap(queue, position, priority_condition);

#ifdef DEBUG
    h_print_queue(queue);
#endif
}

static void min_heapify(heap_priority_queue_t * queue, size_t position, int (*condition)(const TCB_t *, const TCB_t *)) {
    const size_t l = left(position);
    const size_t r = right(position);
    size_t best_priority = position;

    if(l < queue->size && condition(queue->tcb[position], queue->tcb[l]))
        best_priority = l;

    if(r < queue->size && condition(queue->tcb[best_priority], queue->tcb[r]))
        best_priority = r;

    if(best_priority != position) {
        swap_elements(&queue->tcb[0], &queue->tcb[best_priority]);
        min_heapify(queue, best_priority, condition);
    }
}

TCB_t *h_dequeue(heap_priority_queue_t * queue, int (*pop_condition)(const TCB_t *), int (*priority_condition)(const TCB_t *, const TCB_t *)) {
    TCB_t *root = h_peek(queue, pop_condition);

    if(root == 0x00)
        return root;

    if(queue->size == 1) {
        queue->size--;
        return root;
    }

    queue->tcb[0] = queue->tcb[queue->size - 1];
    queue->size--;

    min_heapify(queue, 0, priority_condition);

#ifdef DEBUG
    h_print_queue(queue);
#endif

    return root;
}

TCB_t *h_peek(const heap_priority_queue_t * queue, int (*pop_condition)(const TCB_t *)) {
    if(queue->size == 0 || !pop_condition(queue->tcb[0]))
        return 0x00;

    return queue->tcb[0];
}

size_t h_get_size(const heap_priority_queue_t * queue) {
    return queue->size;
}

short h_is_empty(const heap_priority_queue_t *queue) {
    return queue->size == 0;
}

void h_print_queue(const heap_priority_queue_t * queue) {
    printf("size: %llu\n", queue->size);
    print_task(queue->tcb[0]);
    printf("\n");
}
