//
// Created by Denis on 10/21/2025.
//

#include "mutex.h"

void wait_task(custom_mutex_t *mutex) {
    pthread_mutex_lock(&mutex->mutex);
    while(mutex->mutex_state == MUTEX_LOCKED) {}
    mutex->mutex_state = MUTEX_LOCKED;
    pthread_mutex_unlock(&mutex->mutex);
}

void release_task(custom_mutex_t *mutex) {
    mutex->mutex_state = MUTEX_UNLOCKED;
}

void custom_mutex_init(custom_mutex_t *mutex) {
    mutex->mutex_state = MUTEX_UNLOCKED;
    pthread_mutex_init(&mutex->mutex, NULL);
}

void custom_mutex_lock(custom_mutex_t *mutex) {

}

void custom_mutex_unlock(custom_mutex_t *mutex) {

}

void custom_mutex_destroy(custom_mutex_t *mutex) {

}