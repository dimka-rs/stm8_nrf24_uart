#include "stm8s.h"
#include "main.h"
#include "nrf24.h"

/* Addresses ********************************************************/
#define ADDR_SIZE 5
const uint8_t addr_a[ADDR_SIZE] = {0x1A, 0x1B, 0x1C, 0x1D, 0x1E};
const uint8_t addr_b[ADDR_SIZE] = {0x2A, 0x2B, 0x2C, 0x2D, 0x2E};


/* Platform dependent functions *************************************/
uint8_t SPI_ReadByte();  			//read byte from SPI
void SPI_WriteByte(uint8_t Byte);	//write byte to SPI
void SPI_Start();					//start SPI transaction
void SPI_Stop();					//stop SPI transaction
void CE_Set();						//assert CE
void CE_Reset();					//deassert CE
void DelayMs();						//delay in ms


/* Private functions ************************************************/
static void NRF_SendFrame(uint8_t * Frame, uint8_t FrameSize)
{
	uint8_t i;
	SPI_Start();
	SPI_WriteByte(W_TX_PAYLOAD);
	for(i = 0; i < FrameSize; i++)
	{
		SPI_WriteByte(Frame[i]);
	}
	SPI_Stop();
	CE_Set();
	DelayMs(CE_MIN_HIGH);
	CE_Reset();
}

static uint8_t NRF_ReadReg(uint8_t RegAddr)
{
	uint8_t RegValue;
	SPI_Start();
	SPI_WriteByte(RegAddr);
	RegValue = SPI_ReadByte();
	SPI_Stop();
	return RegValue;
}

static void NRF_WriteReg(uint8_t RegAddr, uint8_t RegValue)
{
	SPI_Start();
	SPI_WriteByte(RegAddr + REG_WMASK);
	SPI_WriteByte(RegValue);
	SPI_Stop();
}

static void NRF_WriteAddress(uint8_t RegAddr, uint8_t * Address, uint8_t AddrLen)
{
	SPI_Start();
	SPI_WriteByte(RegAddr);
	for(uint8_t i = 0; i < AddrLen; i++)
	{
		SPI_WriteByte(*Address[i]);
	}
	SPI_Stop();
}

static void NRF_WritePayload(uint8_t * Buffer, uint8_t Size)
{
	SPI_Start();
	SPI_WriteByte(W_TX_PAYLOAD);
	for(uint8_t i = 0; i < Size; i++)
	{
		SPI_WriteByte(*Buffer[i]);
	}
	SPI_Stop();

}

static void NRF_FlushTx()
{
	SPI_Start();
	SPI_WriteByte(FLUSH_TX);
	SPI_Stop();
}

/* Public functions *************************************************/
void NRF_Init(NRF_InitStructure * Init);
{
	NRF_WriteReg(SETUP_AW, Init->AddrWidth-2); //convert to proper register bits
	NRF_WriteAddress(TX_ADDR, Init->TxAddr, Init->AddrWidth);
	NRF_WriteAddress(RX_ADDR_P0, Init->RxAddrP0, Init->AddrWidth);
	NRF_WriteReg(SETUP_RETR, Init->SetupRetr);
    NRF_WriteReg(RF_CH, Init->RfCh);
	NRF_WriteReg(RF_SETUP, Init->RfSetup);
	NRF_WriteReg(FEATURE, Init->Feature);
	NRF_WriteReg(DYNPD, Init->Dynpd);
	NRF_WriteReg(EN_AA, Init->EnAa);
	NRF_WriteReg(CONFIG, Init->Config);
	NRF_WriteReg(RX_PW_P0, Init->RxPwP0);
	NRF_WriteReg(EN_RXADDR, EnRxaddr);
	NRF_FlushTx();
}


void NRF_SendBuffer(uint8_t * Buffer, uint8_t BufferSize)
{
	uint8_t Offset = 0;
	while(BufferSize)
	{
		if(BufferSize > MAX_FRAME_SIZE)
		{
			NRF_SendFrame(Buffer+Offset, MAX_FRAME_SIZE);
			Offset += MAX_FRAME_SIZE;
			BufferSize -= MAX_FRAME_SIZE;
		} else {
			NRF_SendFrame(Buffer+Offset, BufferSize);
			BufferSize = 0;
		}
	}

}

