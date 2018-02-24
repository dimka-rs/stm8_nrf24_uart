#ifndef _NRF_H_
#define _NRF_H_

#define MAX_FRAME_SIZE 32

void NRF_Init(void);
void NRF_SendBuffer(uint8_t * Buffer, uint8_t BufferSize);
void NRF_SendFrame(uint8_t * Frame, uint8_t FrameSize);

#endif
