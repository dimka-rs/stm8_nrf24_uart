#ifndef _SPI_H_
#define _SPI_H_

void InitSPI();
uint8_t SPI_SendByte(uint8_t data);
void SPI_Start();
void SPI_Stop();

#endif