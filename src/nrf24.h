#ifndef _NRF_H_
#define _NRF_H_

/* various constants */
#define MAX_FRAME_SIZE 32	//max tx payload size in bytes
#define CE_MIN_HIGH 15		//minimal CE high time in ms
#define REG_WMASK 0x20
#define ADDR_SIZE 5
/* registers */
#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RXADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS 0x07
#define OBSERVE_TX 0x08
#define RPD 0x09
#define RX_ADDR_P0 0x0A // 5 bytes!
#define RX_ADDR_P1 0x0B // 5 bytes!
#define RX_ADDR_P2 0x0C
#define RX_ADDR_P3 0x0D
#define RX_ADDR_P4 0x0E
#define RX_ADDR_P5 0x0F
#define TX_ADDR 0x10 // 5 bytes!
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17
#define DYNPD 0x1C
#define FEATURE 0x1D
/* commands */
#define R_RX_PL_WID  0x60
#define R_RX_PAYLOAD 0x61 //up to 32 bytes
#define W_TX_PAYLOAD 0xA0 //up to 32 bytes
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2
#define REUSE_TX_PL  0xE3
#define W_ACK_PAYLOAD 0xA8 //add 3 LSB pipe address
#define W_TX_PAYLOAD_NOACK 0xB0

/* Define types */
struct NRF_InitStruct {
	uint8_t Config;
	uint8_t EnAa;
	uint8_t EnRxaddr;
	uint8_t SetupAw;
	uint8_t SetupRetr;
	uint8_t RfCh;
	uint8_t RfSetup;
	uint8_t * RxAddrP0;
	uint8_t * RxAddrP1;
	uint8_t RxAddrP2;
	uint8_t RxAddrP3;
	uint8_t RxAddrP4;
	uint8_t RxAddrP5;
	uint8_t * TxAddr;
	uint8_t RxPwP0;
    uint8_t RxPwP1;
	uint8_t RxPwP2;
	uint8_t RxPwP3;
	uint8_t RxPwP4;
	uint8_t RxPwP5;
	uint8_t Dynpd;
	uint8_t Feature;
};

/* public functions */
void NRF_SendFrame(uint8_t * Frame, uint8_t FrameSize);
uint8_t NRF_ReadReg(uint8_t RegAddr);
void NRF_WriteReg(uint8_t RegAddr, uint8_t RegValue);
void NRF_WriteAddress(uint8_t RegAddr, uint8_t * Address, uint8_t AddrLen);
void NRF_WritePayload(uint8_t * Buffer, uint8_t Size);
void NRF_FlushTx();

#endif