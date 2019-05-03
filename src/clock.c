#include "stm8s.h"

void InitClock()
{
    /* SYSCLK: 16/1 = 16 MHz */
    /* CPUCLK: 16/2 = 8 MHz */

    CLK->CKDIVR = CLK_PRESCALER_HSIDIV1 | (CLK_PRESCALER_CPUDIV2 & 7);

}