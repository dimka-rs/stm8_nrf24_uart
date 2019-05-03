/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "nrf24.h"
#include "stdio.h"

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
void InitClock();
void InitGPIO();
void InitUART();
void InitNrf();

/* Private functions ---------------------------------------------------------*/

void main(void)
{
    InitClock();
    InitGPIO();
    InitNrf();
    InitUART();
    enableInterrupts();

    printf("Start\r\n");

    while (1)
    {
        if(ReadyToSend)
        {
            ReadyToSend = 0;
            printf("Send\r\n");
        }
    }
}

void InitClock()
{
    /* Clock */
    /* SYSCLK: 16/2 = 8 MHz */
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);
    /* CPUCLK: 8/1 = 8 MHz */
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
}

void InitGPIO()
{
    /* LED */
    GPIO_Init(PORT_LED, PIN_LED, GPIO_MODE_OUT_PP_HIGH_SLOW);
    /* NRF_CS */ 
    GPIO_Init(PORT_NRF, PIN_NRF_CS, GPIO_MODE_OUT_PP_HIGH_SLOW);
    /* NRF_CE */
    GPIO_Init(PORT_NRF, PIN_NRF_CE, GPIO_MODE_OUT_PP_HIGH_SLOW);
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

void InitNrf()
{
    NRF_WriteReg(CONFIG, CONFIG_EN_CRC|CONFIG_PRIM_RX|CONFIG_PWR_UP);
    NRF_WriteReg(EN_AA, ENAA_P0);       //pipe 0 autoack
    NRF_WriteReg(EN_RXADDR, ENRXA_P0);  //pipe 0 enabled
    NRF_WriteReg(SETUP_AW, SETUP_AW_5); //addr width 5 bytes
    NRF_WriteReg(SETUP_RETR, SETUP_RETR_ARD_250|SETUP_RETR_ARC_3); //retr delay and count
    NRF_WriteReg(RF_CH, 2);      //TODO: channel?
    NRF_WriteReg(RF_SETUP, RF_SETUP_DR_2M|RF_SETUP_PWR_M0DBM);
    NRF_WriteAddress(RX_ADDR_P0, addr, ADDR_SIZE); //same address for rx and tx
    NRF_WriteAddress(TX_ADDR,    addr, ADDR_SIZE); //
    NRF_WriteReg(RX_PW_P0, PLD_SIZE); // payload size
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
    GPIO_WriteLow(PORT_LED, PIN_LED);
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
        GPIO_WriteHigh(PORT_LED, PIN_LED);
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
