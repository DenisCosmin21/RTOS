#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stddef.h>
#include "queue.h"
#include "task.h"

typedef struct {
    size_t size;      // semaphore count
    queue_t queue;    // blocked tasks
} semaphore_t;

void semaphore_init(semaphore_t *sem, size_t initial);
void semaphore_up(semaphore_t *sem);
void semaphore_down(semaphore_t *sem);
void semaphore_clear(semaphore_t *sem);

#endif
