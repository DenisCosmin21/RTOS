#include <stdio.h>
#include <pthread.h>

int variable = 0;
pthread_mutex_t mutex;

void *task1(void *data) {
    pthread_mutex_lock(&mutex);

    for(int i = 0;i < 100;i++)
        variable++;

    printf("Afisare din task 1 : %d\n", variable);

    variable = 0;

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *task2(void *data) {
    pthread_mutex_lock(&mutex);

    for(int i = 0;i < 57;i++)
        variable++;

    printf("Afisare din task 2 : %d\n", variable);

    variable = 0;

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *task3(void *data) {
    pthread_mutex_lock(&mutex);

    for(int i = 0;i < 89;i++)
        variable++;

    printf("Afisare din task 3 : %d\n", variable);

    variable = 0;

    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(void) {
    pthread_t task_handle1, task_handle2, task_handle3;

    pthread_mutex_init(&mutex, NULL);

    printf("Mutex\n");

    pthread_create(&task_handle1, NULL, task1, NULL);

    pthread_create(&task_handle2, NULL, task2, NULL);

    pthread_create(&task_handle3, NULL, task3, NULL);

    pthread_join(task_handle1, NULL);

    pthread_join(task_handle2, NULL);

    pthread_join(task_handle3, NULL);

    pthread_mutex_destroy(&mutex);

   /* printf("Barrier\n");

    pthread_create(&task_handle1, NULL, task1, NULL);

    pthread_create(&task_handle2, NULL, task2, NULL);

    pthread_create(&task_handle3, NULL, task3, NULL);

    pthread_join(task_handle1, NULL);

    pthread_join(task_handle2, NULL);

    pthread_join(task_handle3, NULL);*/

    return 0;
}
