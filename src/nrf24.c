#include "stm8s.h"
#include "main.h"
#include "spi.h"
#include "nrf24.h"
#include "timer.h"

void NRF_SendData(uint8_t * Frame, uint8_t FrameSize)
{
    uint8_t reg;

    // power down
    reg = NRF_ReadReg(NRF24_CONFIG);
    reg &= ~((uint8_t)CONFIG_PWR_UP);
    NRF_WriteReg(NRF24_CONFIG, reg);

    // set TX mode
    reg &= ~((uint8_t)CONFIG_PRIM_RX);
    reg |= CONFIG_PWR_UP;
    NRF_WriteReg(NRF24_CONFIG, reg);
    TIM4_DelayMs(1); //wait powerup


    // send payload
    NRF_WritePayload(Frame, FrameSize);

    // do tx
    NRF_CE_Pulse();
    
    //power down
    reg &= ~((uint8_t)CONFIG_PWR_UP);
    NRF_WriteReg(NRF24_CONFIG, reg);

    // back to RX
    reg |= (uint8_t)CONFIG_PRIM_RX | (uint8_t)CONFIG_PWR_UP;
    NRF_WriteReg(NRF24_CONFIG, reg);
    TIM4_DelayMs(1); // wait powerup

}

uint8_t NRF_ReadReg(uint8_t RegAddr)
{
    uint8_t RegValue;
    SPI_Start();
    SPI_SendByte(RegAddr);
    RegValue = SPI_SendByte(0xFF);
    SPI_Stop();
    return RegValue;
}

void NRF_WriteReg(uint8_t RegAddr, uint8_t RegValue)
{
    SPI_Start();
    SPI_SendByte(RegAddr + REG_WMASK);
    SPI_SendByte(RegValue);
    SPI_Stop();
}

void NRF_WriteAddress(uint8_t RegAddr, uint8_t * Address, uint8_t AddrLen)
{
    uint8_t i;
    SPI_Start();
    SPI_SendByte(RegAddr);
    for(i = 0; i < AddrLen; i++)
    {
        SPI_SendByte(Address[i]);
    }
    SPI_Stop();
}

void NRF_WritePayload(uint8_t * Buffer, uint8_t size)
{
    uint8_t i;
    SPI_Start();
    SPI_SendByte(NRF24_W_TX_PAYLOAD);
    for(i = 0; i < size; i++)
    {
        SPI_SendByte(Buffer[i]);
    }
    SPI_Stop();
}

void NRF_ReadPayload(uint8_t * Buffer, uint8_t size)
{
    uint8_t pl_wid;
    SPI_Start();
    pl_wid = SPI_SendByte(NRF24_R_RX_PL_WID);
    SPI_Stop();
    //limit read amount to buf size
    if (pl_wid > size) pl_wid = size;
    
    SPI_Start();
    SPI_SendByte(NRF24_R_RX_PAYLOAD);
    for(uint8_t i = 0; i < pl_wid; i++)
    {
        Buffer[i] = SPI_SendByte(0xFF);
    }
    SPI_Stop();
    //TODO: if buf is small some payload will be lost
    NRF_FlushRx();
}

void NRF_FlushRx()
{
    SPI_Start();
    SPI_SendByte(NRF24_FLUSH_RX);
    SPI_Stop();
}

void NRF_FlushTx()
{
    SPI_Start();
    SPI_SendByte(NRF24_FLUSH_TX);
    SPI_Stop();
}

void NRF_CE_Pulse()
{
    NRF_PORT->ODR |= NRF_CE_PIN;
    TIM4_DelayMs(1);
    NRF_PORT->ODR &= ~NRF_CE_PIN;
}

void NRF_CE_Low()
{
    NRF_PORT->ODR &= ~NRF_CE_PIN;
}

void NRF_CE_High()
{
    NRF_PORT->ODR |= NRF_CE_PIN;
}