#ifndef _GPIO_H_
#define _GPIO_H_

void InitGPIO();
void GPIO_WriteHigh(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins);
void GPIO_WriteLow (GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins);

#endif