#include "stm8s.h"
#include "main.h"
#include "spi.h"
#include "nrf24.h"
#include "timer.h"

/* Platform dependent functions *************************************/
void CE_Pulse(); //assert CE for required time


/* Public functions ************************************************/
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
    CE_Pulse();
    
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

void NRF_WritePayload(uint8_t * Buffer, uint8_t Size)
{
    uint8_t i;
    SPI_Start();
    SPI_SendByte(NRF24_W_TX_PAYLOAD);
    for(i = 0; i < Size; i++)
    {
        SPI_SendByte(Buffer[i]);
    }
    SPI_Stop();

}

void NRF_FlushTx()
{
    SPI_Start();
    SPI_SendByte(NRF24_FLUSH_TX);
    SPI_Stop();
}


void CE_Pulse()
{
    NRF_PORT->ODR |= NRF_CE_PIN;
    TIM4_DelayMs(1);
    NRF_PORT->ODR &= ~NRF_CE_PIN;
}