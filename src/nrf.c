#include "stm8s.h"
#include "main.h"
#include "nrf.h"

void NRF_Init()
{
	//TODO: set CS, CE
	// freq channel
	// bw
	// bitrate
	// power
	// addresses


}

void NRF_SendFrame(uint8_t * Frame, uint8_t FrameSize)
{
	uint8_t i;
	for(i = 0; i < FrameSize; i++)
	{
		/* This is test code to check what was received */
		UART1_SendData8(Frame[i]);
	}
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
