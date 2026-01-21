#ifndef SEMAPHORE_H


#define SEMAPHORE_H





#include <stddef.h>





#include "queue.h"


typedef struct TCB TCB_t;





typedef struct semaphore {


    queue_t queue;


    size_t size;


} semaphore_t;





void semaphore_init(semaphore_t *sem, size_t size);
void semaphore_up(semaphore_t *sem);
void semaphore_down(semaphore_t *sem);
void semaphore_clear(semaphore_t *sem);


#endif //SEMAPHORE_H
