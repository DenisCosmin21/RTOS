//
// Created by Denis on 10/21/2025.
//

#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>

typedef enum {
    MUTEX_LOCKED,
    MUTEX_UNLOCKED
}custom_mutex_state;

typedef struct {
    custom_mutex_state mutex_state;
    pthread_mutex_t mutex;
}custom_mutex_t;

//Initializeaza un mutex
void custom_mutex_init(custom_mutex_t *mutex);

//Blocheaza zona critica de program
void custom_mutex_lock(custom_mutex_t *mutex);

//Deblocheaza zona critica de program
void custom_mutex_unlock(custom_mutex_t *mutex);

//Elibereaza memoria ocupata de un mutex
void custom_mutex_destroy(custom_mutex_t *mutex);
#endif //MUTEX_H
