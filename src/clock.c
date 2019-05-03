#include "stm8s.h"

void InitClock()
{
    /* SYSCLK: 16/2 = 8 MHz */
    /* CPUCLK: 8/1 = 8 MHz */

    CLK->CKDIVR = CLK_PRESCALER_HSIDIV2 | (CLK_PRESCALER_CPUDIV1 & 7);

}