/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "nrf24.h"
#include "stdio.h"
#include "clock.h"
#include "gpio.h"

/* Private defines -----------------------------------------------------------*/
#define UART1_BAUDRATE 115200
#define PLD_SIZE 32 //payload size

uint8_t TxBufA[PLD_SIZE];
uint8_t TxBufB[PLD_SIZE];
uint8_t RxBuf[PLD_SIZE];
uint8_t *CurBuf = TxBufA;
uint8_t CurBufCnt = 0;
uint8_t RxDone;
uint8_t ReadyToSend = 0;

nrfctl_t nrf;

#define ADDR_SIZE 5
const uint8_t addr[ADDR_SIZE] = {0x1A, 0x1B, 0x1C, 0x1D, 0x1E};

/* Private function prototypes -----------------------------------------------*/
void InitUART();
void InitSPI();
void InitNRF();
static inline void LED_On();
static inline void LED_Off();


/* Private functions ---------------------------------------------------------*/

void main(void)
{
    InitClock();
    InitGPIO();
    InitUART();
    InitSPI();
    InitNRF();
    enableInterrupts();

    printf("Start\r\n");

    //printf("Status: %02X\r\n", NRF_ReadReg(NRF24_STATUS));
    while (1)
    {
        if(ReadyToSend)
        {
            ReadyToSend = 0;
            printf("Send\r\n");
            //NRF_SendData(nrf.txbufptr, nrf.txbufsize);
        }
    }
}


void InitUART()
{
    /* UART 1 */
    UART1_Init(UART1_BAUDRATE, UART1_WORDLENGTH_8D, UART1_STOPBITS_1,
                UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE,
                UART1_MODE_TXRX_ENABLE);
    UART1_ClearFlag(UART1_FLAG_RXNE);
    /* RXNE fails assert, RXNE_OR works fine */
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
}

/*
void InitTimer4()
{
    // Timer 4
    // 16 MHz / 64 = 250 kHz timer clock
    // count to 25 to get 10 kHz / 0.1 ms resolution
    // one byte over uart takes about 0.1 ms
    TIM4_Cmd(DISABLE);
    TIM4_TimeBaseInit(TIM4_PRESCALER_64, 25);
    TIM4_SelectOnePulseMode(TIM4_OPMODE_SINGLE);
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);	
    TIM4_Cmd(ENABLE);
}
*/

void InitSPI()
{
/*
    SPI_Init(SPI_FIRSTBIT_MSB,
            SPI_BAUDRATEPRESCALER_2,
            SPI_MODE_MASTER,
            SPI_CLOCKPOLARITY_LOW,
            SPI_CLOCKPHASE_1EDGE,
            SPI_DATADIRECTION_2LINES_FULLDUPLEX,
            SPI_NSS_SOFT,
            0);
            */
}

void InitNRF()
{
    NRF_WriteReg(NRF24_CONFIG, CONFIG_EN_CRC|CONFIG_PRIM_RX|CONFIG_PWR_UP);
    NRF_WriteReg(NRF24_EN_AA, ENAA_P0);       //pipe 0 autoack
    NRF_WriteReg(NRF24_EN_RXADDR, ENRXA_P0);  //pipe 0 enabled
    NRF_WriteReg(NRF24_SETUP_AW, SETUP_AW_5); //addr width 5 bytes
    NRF_WriteReg(NRF24_SETUP_RETR, SETUP_RETR_ARD_250|SETUP_RETR_ARC_3); //retr delay and count
    NRF_WriteReg(NRF24_RF_CH, 2);      //TODO: channel?
    NRF_WriteReg(NRF24_RF_SETUP, RF_SETUP_DR_2M|RF_SETUP_PWR_M0DBM);
    NRF_WriteAddress(NRF24_RX_ADDR_P0, addr, ADDR_SIZE); //same address for rx and tx
    NRF_WriteAddress(NRF24_TX_ADDR,    addr, ADDR_SIZE); //
    NRF_WriteReg(NRF24_RX_PW_P0, PLD_SIZE); // payload size
}

static inline void LED_On() {
    GPIO_WriteLow(LED_PORT, LED_PIN);
}

static inline void LED_Off() {
    GPIO_WriteHigh(LED_PORT, LED_PIN);
}

/***********************************************************************/

/*
void tim4_isr(void) __interrupt(ITC_IRQ_TIM4_OVF) {
    static uint8_t tim4cnt = 0;
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
    tim4cnt++;
    if(tim4cnt >= RX_TOUT){
        TIM4_Cmd(DISABLE); //TODO: remove if one pulse mode really works
        tim4cnt = 0;
        RxDone = 1;
    }   
}
*/

void uart1rx_isr(void) __interrupt(ITC_IRQ_UART1_RX) {
    UART1_ClearFlag(UART1_FLAG_RXNE);
    LED_On();
//    TIM4_Cmd(ENABLE);
//    TIM4_SetCounter(0);
    CurBuf[CurBufCnt] = UART1_ReceiveData8();
    CurBufCnt++;
    if(CurBufCnt >= PLD_SIZE)
    {
        if(CurBuf == TxBufA)
        {
            CurBuf = TxBufB;
            nrf.txbufptr = TxBufA;
        } else {
            CurBuf = TxBufA;
            nrf.txbufptr = TxBufB;
        }
        nrf.txbufsize = CurBufCnt;
        CurBufCnt = 0;
        ReadyToSend = 1;
        LED_Off();
    }
}

/* Platform dependent functions */

uint8_t SPI_SendByte(uint8_t Byte) {
    //read/write byte from/to SPI
    return (uint8_t) Byte;
}

void SPI_Start() {
    //start SPI transaction
}

void SPI_Stop() {
    //stop SPI transaction
}

void CE_Pulse() {
    //assert CE for required time
}

/* Debug output*/

int putchar (int c)
{
    // this implementation is SDCC 3.8 specific
    // see sdcc stdio.h for details
    UART1->DR = (uint8_t) c;
    while (!(UART1->SR & UART1_FLAG_TXE));
    return c;
}


// This is called by some of the SPL files on error.
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  (void)file;
  (void)line;

  /* User can add their own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
