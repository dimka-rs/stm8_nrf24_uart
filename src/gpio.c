#include "stm8s.h"
#include "main.h"

void InitGPIO()
{
    /* LED out push-pull high slow*/
    LED_PORT->ODR |= (uint8_t)LED_PIN;      // default high
    LED_PORT->CR1 |= (uint8_t)LED_PIN;      // push-pull
    LED_PORT->CR2 &= (uint8_t)(~LED_PIN);   // slow
    LED_PORT->DDR |= (uint8_t)LED_PIN;      // output

    /* NRF_CS out push-pull high slow */ 
    NRF_PORT->ODR |= (uint8_t)NRF_CS_PIN;       // default high
    NRF_PORT->CR1 |= (uint8_t)NRF_CS_PIN;       // push-pull
    NRF_PORT->CR2 &= (uint8_t)(~NRF_CS_PIN);    // slow
    NRF_PORT->DDR |= (uint8_t)NRF_CS_PIN;       // output

    /* NRF_CE out puysh-pull low slow */
    NRF_PORT->ODR &= (uint8_t)(~NRF_CE_PIN);    // default low
    NRF_PORT->CR1 |= (uint8_t)NRF_CE_PIN;       // push-pull
    NRF_PORT->CR2 &= (uint8_t)(~NRF_CE_PIN);    // slow
    NRF_PORT->DDR |= (uint8_t)NRF_CE_PIN;       // output

    /* NRF_IRQ input pull-up no int */
    NRF_IRQ_PORT->DDR &= (uint8_t)(~(NRF_IRQ_PIN)); //input
    NRF_IRQ_PORT->CR1 |= (uint8_t)NRF_IRQ_PIN;      //pull-up
    NRF_IRQ_PORT->CR2 &= (uint8_t)(~(NRF_IRQ_PIN)); //no int
}

void GPIO_WriteHigh(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins)
{
  GPIOx->ODR |= (uint8_t)PortPins;
}

void GPIO_WriteLow(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins)
{
  GPIOx->ODR &= (uint8_t)(~PortPins);
}