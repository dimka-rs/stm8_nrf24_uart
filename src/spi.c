#include "stm8s.h"
#include "main.h"
#include "spi.h"
#include "timer.h"

void InitSPI()
{
    /* minimal prescaler = 2, so CLK is 8 MHz */
    SPI->CR1 = 4; // MSB fist , /2, master, pol 0, phase 0
    SPI->CR2 = 0; // 2 lines, no crc, FD, app cs

    SPI->CR1 |= SPI_CR1_SPE;
}

uint8_t SPI_SendByte(uint8_t data)
{
    SPI->DR = data;
    while(!(SPI->SR & SPI_SR_TXE));
    while(!(SPI->SR & SPI_SR_RXNE));
    return SPI->DR;
}

void SPI_Start()
{
    NRF_PORT->ODR &= ~NRF_CS_PIN;
    TIM4_DelayMs(1);
}

void SPI_Stop()
{
    NRF_PORT->ODR |= NRF_CS_PIN;
    TIM4_DelayMs(1);
}