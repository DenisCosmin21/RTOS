#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <stdint.h>
#include "semaphore.h"

#define MSG_QUEUE_SIZE 8

typedef struct {
    void *buffer[MSG_QUEUE_SIZE];
    uint32_t head;
    uint32_t tail;
    uint32_t count; // Active element count

    semaphore_t lock;       // mutex

    semaphore_t not_empty;  // items available
    semaphore_t not_full;   // free slots
} message_queue_t;

void msg_queue_init(message_queue_t *queue);

int msg_queue_send(
    message_queue_t *queue,
    void *message,
    uint32_t timeout_ms   // currently ignored
);

int msg_queue_receive(
    message_queue_t *queue,
    void **message,
    uint32_t timeout_ms   // currently ignored
);

#endif
