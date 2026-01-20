#include "message_queue.h"

// Init
void msg_queue_init(message_queue_t *queue) {
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;

    semaphore_init(&queue->lock, 1);
    semaphore_init(&queue->not_empty, 0);
    semaphore_init(&queue->not_full, MSG_QUEUE_SIZE);
}

int msg_queue_send(message_queue_t *queue, void *message, uint32_t timeout_ms) {
    (void)timeout_ms;


    semaphore_down(&queue->not_full);
    semaphore_down(&queue->lock);

    queue->buffer[queue->tail] = message;
    queue->tail = (queue->tail + 1) % MSG_QUEUE_SIZE;
    queue->count++;

    semaphore_up(&queue->lock);
    semaphore_up(&queue->not_empty);

    return 1;
}

int msg_queue_receive(message_queue_t *queue, void **message, uint32_t timeout_ms) {
    (void)timeout_ms;
    semaphore_down(&queue->not_empty);
    semaphore_down(&queue->lock);

    *message = queue->buffer[queue->head];
    queue->head = (queue->head + 1) % MSG_QUEUE_SIZE;
    queue->count--;

    semaphore_up(&queue->lock);
    semaphore_up(&queue->not_full);

    return 1;
}

