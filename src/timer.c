#include "stm8s.h"
#include "timer.h"


//timer 4 counter
volatile uint16_t tim4cnt;

void InitTIM4()
{
    // Timer 4
    TIM4->CR1 &= (uint8_t)(~TIM4_CR1_CEN); // disable

    // 16 MHz / 64 = 250 kHz timer clock
    // count to 25 to get 10 kHz / 0.1 ms resolution
    TIM4->PSCR = (uint8_t)(TIM4_PRESCALER_64);
    TIM4->ARR  = (uint8_t)(25);
    TIM4->SR1 &= (uint8_t)(~TIM4_FLAG_UPDATE); // clear it flag
    TIM4->IER |= (uint8_t)TIM4_IT_UPDATE; //enable it
}

void TIM4_DelayMs(uint16_t delay)
{
    tim4cnt = delay * 10;
    TIM4->CR1 |= (uint8_t)TIM4_CR1_CEN; //enable
    while(tim4cnt);
    TIM4->CR1 &= (uint8_t)(~TIM4_CR1_CEN); // disable
}