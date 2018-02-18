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

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Init();

void main(void)
{
	Init();
  
  while (1)
  {
    // Toggle a bit in the output data register to blink the LED
	//GPIO_WriteReverse(PORT_LED, PIN_LED); 
  }
}

void Init()
{
	/* Clock */
	//TODO: set 16 MHz
	
    /* LED */
	GPIO_Init(PORT_LED, PIN_LED, GPIO_MODE_OUT_PP_LOW_SLOW);
	/* NRF_CS */ 
	GPIO_Init(PORT_NRF, PIN_NRF_CS, GPIO_MODE_OUT_PP_HIGH_SLOW);
	/* NRF_CE */
  	GPIO_Init(PORT_NRF, PIN_NRF_CE, GPIO_MODE_OUT_PP_HIGH_SLOW);

	/* Timer 4 */
	/* 2 MHz / 16 = 125000, so period is 125 to get 1 ms */
    TIM4_Cmd(DISABLE);
    TIM4_TimeBaseInit(TIM4_PRESCALER_16, 125);
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);	
	TIM4_Cmd(ENABLE);

	enableInterrupts();
}

void tim4_isr(void) __interrupt(ITC_IRQ_TIM4_OVF) {
    static uint16_t cnt = 0;
    cnt++;
    if(cnt > 1000){
        cnt = 0;
        GPIO_WriteReverse(GPIOB, GPIO_PIN_5);
    }   
    TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
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
