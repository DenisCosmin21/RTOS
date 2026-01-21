#include "led.h"

#define GPIOBEN 		(1U<<1)
#define LED_PIN 		(1U<<7)

void led_init(void){
	/*Enable clock access to led port ( port )*/

	RCC->AHB2ENR1 |= GPIOBEN;

	/*Set LED pin as output pin*/
	//14-15

	GPIOB->MODER |= (1U<<14);
	GPIOB->MODER  &= ~(1U<<15);
}


void led_on(void){

	/*Set led pin HIGH(PB7)*/
	GPIOB->ODR |= LED_PIN;
}

void led_off(void){

	/*Set led pin LOW(PB7)*/
	GPIOB->ODR &= ~LED_PIN;
}

void led_toggle(void){
    GPIOB->ODR ^= LED_PIN;
}

