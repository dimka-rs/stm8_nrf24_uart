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
#include "nrf.h"

/* Private defines -----------------------------------------------------------*/
#define RX_TOUT 10  // num of tim4 overflows, which are about 0.1 ms or 1 symbol
#define RX_BUF_SIZE 256
uint8_t RxBuf[RX_BUF_SIZE];
uint8_t RxOffset = 0;
uint8_t RxDone;

/* Private function prototypes -----------------------------------------------*/
void Init();

/* Private functions ---------------------------------------------------------*/

void main(void)
{
	Init();
  
	while (1)
	{
		while(RxDone)
		{
			RxDone = 0;
			NRF_SendBuffer(RxBuf, RxOffset);
			RxOffset = 0;
		}

	}
}

void Init()
{
	/* Clock */
	/* SYSCLK = 16 MHz */
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	/* CPUCLK = 8 MHz */
	CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV2);
	
    /* LED */
	GPIO_Init(PORT_LED, PIN_LED, GPIO_MODE_OUT_PP_LOW_SLOW);
	/* NRF_CS */ 
	GPIO_Init(PORT_NRF, PIN_NRF_CS, GPIO_MODE_OUT_PP_HIGH_SLOW);
	/* NRF_CE */
  	GPIO_Init(PORT_NRF, PIN_NRF_CE, GPIO_MODE_OUT_PP_HIGH_SLOW);

	/* UART 1 */
	UART1_Init(115200, UART1_WORDLENGTH_8D, UART1_STOPBITS_1,
				UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE,
				UART1_MODE_TXRX_ENABLE);
	UART1_ITConfig(UART1_IT_RXNE, ENABLE);
	UART1_Cmd(ENABLE);

	/* Timer 4 */
	/* 16 MHz / 64 = 250 kHz timer clock */
	/* count to 25 to get 10 kHz / 0.1 ms resolution */
	/* one byte over uart takes about 0.1 ms */
    TIM4_Cmd(DISABLE);
    TIM4_TimeBaseInit(TIM4_PRESCALER_64, 25);
	TIM4_SelectOnePulseMode(TIM4_OPMODE_SINGLE);
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);	
	TIM4_Cmd(ENABLE);

	enableInterrupts();
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
	TIM4_Cmd(ENABLE);
	TIM4_SetCounter(0);
	RxBuf[RxOffset] = UART1_ReceiveData8();
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
