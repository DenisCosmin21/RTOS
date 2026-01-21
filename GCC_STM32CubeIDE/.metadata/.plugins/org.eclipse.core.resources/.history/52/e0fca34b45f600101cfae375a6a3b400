/*


#include "globals.h"
#include "led.h"
#include "mutex.h"
#include "osKernel.h"
#include "rtos.h"
#include "uart.h"
#include <stdint.h>
#include <stdio.h>


volatile int pi_enabled = 0;
volatile int start_low = 0;
volatile int extra_load = 0;

mutex_t demo_mutex;
mutex_t uart_mutex;

void busy_wait(int loops) {
    for (volatile int i = 0; i < loops + extra_load; i++);
}


void task_high(void) {
    while (1) {
        uint32_t t_start = rtos_now();

        mutex_lock(&demo_mutex);
        busy_wait(1000); // ~4–5 ms (sub deadline)
        mutex_unlock(&demo_mutex);

        uint32_t t_end = rtos_now();

        mutex_lock(&uart_mutex);
        if((t_end - t_start) > 20) {
            uart_printf("[H] DEADLINE MISS! dt=%d ms\r\n", t_end - t_start);
            led_on();
        } else {
            uart_printf("[H] OK dt=%d ms\r\n", t_end - t_start);
            led_off();
        }
        mutex_unlock(&uart_mutex);

        rtos_task_wait();
    }
}


void task_med(void) {
    while(1) {
        mutex_lock(&uart_mutex);
        uart_printf("[M] Run\r\n");
        mutex_unlock(&uart_mutex);

        busy_wait(8000); // ~8–10 ms
        rtos_task_wait();
    }
}


void task_low(void) {
    while(1) {
        if(!start_low) {
            rtos_task_wait();
            continue;
        }

        mutex_lock(&uart_mutex);
        uart_printf("[L] Trying mutex\r\n");
        mutex_unlock(&uart_mutex);


        mutex_lock(&demo_mutex);
        busy_wait(6000);   // ~6 ms
        mutex_unlock(&demo_mutex);
        uart_printf("[L] Done\r\n");

        mutex_lock(&uart_mutex);
        uart_printf("[L] Done\r\n");
        mutex_unlock(&uart_mutex);

        rtos_task_wait();
    }
}

void task_controller(void) {
    static int sec = 0;

    while (1) {
        mutex_lock(&uart_mutex);

        if (sec == 0) {
            uart_printf("\n1: High ann Low running\r\n");
        }
        else if (sec == 2) {
            uart_printf("\n2:Priority Inversion OFF\r\n");
            start_low = 1;
        }
        else if (sec == 4) {
            uart_printf("\n3: Priority Inversion ON\r\n");
            pi_enabled = 1;
        }
        else if (sec == 6) {
            uart_printf("\n4:Increase load\r\n");
            extra_load = 4000;
        }

        sec++;
        mutex_unlock(&uart_mutex);
        rtos_task_wait();
    }
}



int main(void) {
    uart_tx_init();
    led_init();

    mutex_init(&demo_mutex);
    mutex_init(&uart_mutex);

    osKernelInit();

    uart_printf("Booting RTOS Tick rate = 1000 Hz\r\n");



    rtos_task_create(task_high,  2,   25,   1024, "HIGH");
    rtos_task_create(task_med,   10,   30,   1024, "MED");
    rtos_task_create(task_low,   5,   90,   1024, "LOW");
    rtos_task_create(task_controller, 2, 1000, 1024, "CTRL");

    rtos_start();
    return 0;
}



*/
