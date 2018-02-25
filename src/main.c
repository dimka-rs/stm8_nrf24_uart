/**
  ******************************************************************************
  * @file    Project/main.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "nrf24.h"

/* Private defines -----------------------------------------------------------*/
#define UART1_BAUDRATE 115200
#define RX_TOUT 10  // num of tim4 overflows, which are about 0.1 ms or 1 symbol
#define RX_BUF_SIZE 256
uint8_t RxBuf[RX_BUF_SIZE];
uint8_t RxOffset = 0;
uint8_t RxDone;

#define ADDR_SIZE 5
const uint8_t addr_a[ADDR_SIZE] = {0x1A, 0x1B, 0x1C, 0x1D, 0x1E};
const uint8_t addr_b[ADDR_SIZE] = {0x2A, 0x2B, 0x2C, 0x2D, 0x2E};

/* Private function prototypes -----------------------------------------------*/
void Init();
void InitNrf();

/* Private functions ---------------------------------------------------------*/

void main(void)
{
	Init();
	InitNrf();
  
	while (1)
	{
		//while(UART1_GetFlagStatus(UART1_FLAG_TXE) != SET);
		while(RxDone)
		{
			GPIO_WriteHigh(PORT_LED, PIN_LED);
			RxDone = 0;
			NRF_SendBuffer(RxBuf, RxOffset);
			RxOffset = 0;
		}

	}
}

void Init()
{
	/* Clock */
	/* SYSCLK: 16/1 = 16 MHz */
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	/* CPUCLK: 16/2 = 8 MHz */
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV2);
	
    /* LED */
	GPIO_Init(PORT_LED, PIN_LED, GPIO_MODE_OUT_PP_HIGH_SLOW);
	/* NRF_CS */ 
	GPIO_Init(PORT_NRF, PIN_NRF_CS, GPIO_MODE_OUT_PP_HIGH_SLOW);
	/* NRF_CE */
  	GPIO_Init(PORT_NRF, PIN_NRF_CE, GPIO_MODE_OUT_PP_HIGH_SLOW);
	/* ADDRESS PIN */
	GPIO_Init(PORT_ADDR, PIN_ADDR_1, GPIO_MODE_IN_PU_NO_IT);

	/* UART 1 */
	UART1_Init(UART1_BAUDRATE, UART1_WORDLENGTH_8D, UART1_STOPBITS_1,
				UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE,
				UART1_MODE_TXRX_ENABLE);
	UART1_ClearFlag(UART1_FLAG_RXNE);
	/* RXNE fails assert, RXNE_OR works fine */
	UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);

	/* Timer 4 */
	/* 16 MHz / 64 = 250 kHz timer clock */
	/* count to 25 to get 10 kHz / 0.1 ms resolution */
	/* one byte over uart takes about 0.1 ms */
    TIM4_Cmd(DISABLE);
    TIM4_TimeBaseInit(TIM4_PRESCALER_64, 25);
	//TIM4_SelectOnePulseMode(TIM4_OPMODE_SINGLE);
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);	
	//TIM4_Cmd(ENABLE);

	enableInterrupts();
}

void InitNrf()
{
	struct NRF_InitStruct InitTx;
	InitTx.Config = 0x0A; //CRC 1 byte, power up, PTX
	InitTx.EnAa = 0x3F; //auto ack on all pipes
	InitTx.EnRxaddr = 0x01; //pipes 0 enabled
	InitTx.SetupAw = 0x03;  //addr width 5 bytes
	InitTx.SetupRetr = 0x03; // art delay 250 us, art count 3
	InitTx.RfCh = 0x02; //TODO: channel?
	InitTx.RfSetup = 0x0E; //2 Mbit, 0 dBm
	InitTx.RxAddrP0 = addr_a;
	InitTx.RxAddrP1 = addr_b; //not used
	InitTx.RxAddrP2 = 0x00;
	InitTx.RxAddrP3 = 0x00;
	InitTx.RxAddrP4 = 0x00;
	InitTx.RxAddrP5 = 0x00;
	InitTx.TxAddr = addr_a;
	InitTx.RxPwP0 = 0x00;
    InitTx.RxPwP1 = 0x00;
	InitTx.RxPwP2 = 0x00;
	InitTx.RxPwP3 = 0x00;
	InitTx.RxPwP4 = 0x00;
	InitTx.RxPwP5 = 0x00;
	InitTx.Dynpd = 0x01; //dyn payload on pipe 0
	InitTx.Feature = 0x04; //enable dyn payload
}

/***********************************************************************/

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

void uart1rx_isr(void) __interrupt(ITC_IRQ_UART1_RX) {
	UART1_ClearFlag(UART1_FLAG_RXNE);
	GPIO_WriteLow(PORT_LED, PIN_LED);
	TIM4_Cmd(ENABLE);
	TIM4_SetCounter(0);
	RxBuf[RxOffset] = UART1_ReceiveData8();
	RxOffset++;
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
