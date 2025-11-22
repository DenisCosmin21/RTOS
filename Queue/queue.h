//
// Created by Denis on 10/27/2025.
//

#ifndef QUEUE_H
#define QUEUE_H
#define QUEUE_SIZE 10
#include "../Task/task.h"

typedef struct {
    TCB_t tcb[QUEUE_SIZE];
    int front;
    int rear;
    int size;
}queue_t;

void queue_init(queue_t *);

//Adds a task to the queue
void enqueue(queue_t *,const TCB_t);

//Removes and returns a task from the queue
TCB_t dequeue(queue_t *);

//Peeks to the last task from the queue, returning it without remvoing it from the queue
TCB_t peek(const queue_t *);

int queue_is_empty(const queue_t *);

void print_queue(const queue_t *queue);
#endif //QUEUE_H
