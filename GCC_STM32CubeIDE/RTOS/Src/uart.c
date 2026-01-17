#include "uart.h"
#include "stm32u5xx.h"
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "globals.h"
#include "rms_scheduler.h"
#include "rtos.h"
#include "semaphore.h"

#define UART_BUFFER_SIZE 256

#define GPIOAEN     (1U << 0)
#define UART1EN     (1U << 14)
#define GPDMA1EN    (1U << 0)

#define SYS_FREQ        4000000UL
#define APB2_CLK        SYS_FREQ
#define UART_BAUDRATE   115200

#define CR1_TE      (1U << 3)
#define CR1_UE      (1U << 0)
#define CR3_DMAT    (1U << 7)

#define ISR_TXE     (1U << 7)

#define GPDMA_CCR_EN      (1U << 0)
#define GPDMA_CSR_IDLEF   (1U << 0)
#define GPDMA_CTR2_REQSEL_POS (0U)

#define USART1_TX_DMA_REQ_ID  25

semaphore_t uart_sem;

static void uart_write(int ch);
__attribute__((aligned(32)))
static uint8_t dma_buffer[UART_BUFFER_SIZE];



static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate) {
	return ((periph_clk + (baudrate/2U))/baudrate);
}
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate){
	USART1->BRR = compute_uart_bd(periph_clk, baudrate);
}

static void my_CleanDCache(uint32_t *addr, int32_t dsize) {
    if (dsize < 0) return;
    uint32_t op_size = (uint32_t)dsize;
    uint32_t op_addr = (uint32_t)addr;
    int32_t linesize = 32;

    op_size += (op_addr & (linesize - 1U));
    op_addr &= ~(linesize - 1U);

    while(op_size >0U) {
        *(volatile uint32_t *)0xE000EF68 = op_addr;
        op_addr+= linesize;
        op_size-= (op_size >= linesize) ? linesize : op_size;
    }
    __asm volatile ("dsb 0xF":::"memory");
}

void dma_init(void) {
    RCC->AHB1ENR |=GPDMA1EN;
    GPDMA1_Channel0->CCR &=  ~GPDMA_CCR_EN;
    NVIC_SetPriority(GPDMA1_Channel0_IRQn, 5);
    NVIC_EnableIRQ(GPDMA1_Channel0_IRQn);
}

void uart_tx_init(void)
{
    semaphore_init(&uart_sem, 1);

    RCC->AHB2ENR1 |= GPIOAEN;

    GPIOA->MODER &= ~(1U << 18);
    GPIOA->MODER |= (1U << 19);

    GPIOA->AFR[1] |=(1U << 4);
    GPIOA->AFR[1] |=(1U << 5);
    GPIOA->AFR[1] |=(1U << 6);
    GPIOA->AFR[1] &= ~(1U << 7);

    RCC->APB2ENR|= UART1EN;

    uart_set_baudrate(APB2_CLK, UART_BAUDRATE);
    USART1->CR1= CR1_TE;
    USART1->CR3 |= CR3_DMAT;
    USART1->CR1 |= CR1_UE;

    dma_init();
}

static void uart_write(int ch){
    while(!(USART1->ISR & ISR_TXE)){}
    USART1->TDR = (ch & 0xFF);
}

void uart_send_dma(uint8_t *data, uint16_t  len)
{
    my_CleanDCache((uint32_t*)data, len);
    GPDMA1_Channel0->CCR &= ~GPDMA_CCR_EN;
    while( !(GPDMA1_Channel0->CSR & GPDMA_CSR_IDLEF) );
    GPDMA1_Channel0->CFCR= 0xFFFFFFFF;


    GPDMA1_Channel0->CTR1 = (1U << 3);
    GPDMA1_Channel0->CTR2= (USART1_TX_DMA_REQ_ID << GPDMA_CTR2_REQSEL_POS);
    GPDMA1_Channel0->CBR1=len;

    GPDMA1_Channel0->CSAR= (uint32_t)data;
    GPDMA1_Channel0->CDAR= (uint32_t)&USART1->TDR;
    GPDMA1_Channel0->CCR |= (1U << 8) | (1U << 9) | GPDMA_CCR_EN;
}

/*
static void uart_send_dma_wait(void) {
    if(uart_blocked_task == 0) {
    	return;
    }
       next_task = scheduler_get_task();
  //  SCB->ICSR |=  SCB_ICSR_PENDSVSET_Msk;
       context_switch();
}

*/

void uart_printf(const char *format, ...) {
   // semaphore_down(&uart_sem); // Wait until previous DMA is done
    va_list args;
    va_start(args, format);
    int len = vsnprintf((char*)dma_buffer, UART_BUFFER_SIZE, format, args);
    va_end(args);
    if(len> 0) {
       if(len >UART_BUFFER_SIZE)
        len = UART_BUFFER_SIZE;
        uart_send_dma(dma_buffer, (uint16_t)len);
    } else {
        //semaphore_up(&uart_sem); // Release if nothing to send
    }
}

// aici o sa vina logica de semafor
void GPDMA1_CH0_IRQHandler(void) {
    uint32_t status = GPDMA1_Channel0->CSR;

    if(status & ((1U << 8) | (1U << 9))) {
       GPDMA1_Channel0->CFCR = 0xFFFFFFFF;
       semaphore_up(&uart_sem); // Signal transfer complete
    }
}
int __io_putchar(int ch){
    uart_write(ch);
    return ch;
}
