#include "stm8s.h"
#include "main.h"
#include "nrf24.h"

/* Platform dependent functions *************************************/
uint8_t SPI_SendByte(uint8_t Byte);	//read/write byte from/to SPI
void SPI_Start();					//start SPI transaction
void SPI_Stop();					//stop SPI transaction
void CE_Pulse();					//assert CE for required time


/* Public functions ************************************************/
void NRF_SendFrame(uint8_t * Frame, uint8_t FrameSize)
{
	NRF_WritePayload(Frame, FrameSize);
	CE_Pulse();
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
	SPI_SendByte(W_TX_PAYLOAD);
	for(i = 0; i < Size; i++)
	{
		SPI_SendByte(Buffer[i]);
	}
	SPI_Stop();

}

void NRF_FlushTx()
{
	SPI_Start();
	SPI_SendByte(FLUSH_TX);
	SPI_Stop();
}
