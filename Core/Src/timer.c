/*
 * timer.c
 *
 *  Created on: Apr 12, 2025
 *      Author: MadaYaswanth
 */
#include"main.h"
#include"timer.h"
#include<stdint.h>
#include<stdio.h>
#include"can_data_array1.h"
uint8_t TIMER_FLAG=0;
uint32_t last1=0;
uint32_t last_tick=0;
uint32_t ar_index=0;
extern uint8_t CAN_Send(const uint32_t id,const  uint8_t is_std_etd,const uint8_t *data);
extern void uart_int(int n);
extern void uart_string(char *p);
extern void uart_tx(uint8_t c);
void timer_init(){
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable clock for TIM2 and AFIO
    TIM2->PSC =72 - 1;// 72 - 1; // Set TIM2 prescaler to get 1 MHz timer clock (72 MHz / 72 = 1 MHz -> 1 tick = 1 us)
    TIM2->ARR = can_data[0].delay_ms-1;
    TIM2->CNT = 0;
    TIM2->EGR |= TIM_EGR_UG;
    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1 |= TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM2_IRQn);
}
void TIM2_IRQHandler(void) {

    if(TIM2->SR & TIM_SR_UIF)// Update interrupt flag
    {
    	 TIM2->SR &= ~TIM_SR_UIF; // Clear interrupt flag
		CAN_Send( can_data[ar_index].id,can_data[ar_index].is_extended,can_data[ar_index].data);
		ar_index++;
		if (ar_index>=(ARRAY_SIZE-1)) {
			  ar_index = 0;  // Restart or stop
		 }
		TIM2->PSC=72-1;
		TIM2->ARR = can_data[ar_index].delay_ms*100-1; // Set next delay
		TIM2->CNT = 0;  // Reset counter
    }

}



