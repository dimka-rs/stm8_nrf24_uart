#include "stm8s.h"
#include "main.h"

void InitUART()
{
    /* UART 1 */

    // UART_DIV = f MASTER / baud rate
    // UART_DIV = 8 * 10^6 / 115200
    // UART_DIV = 69.44 = 0x0045 = 0x0 (BRR2_H), 0x04 (BRR1), 0x05 (BRR2_L)

    UART1->BRR2 = 0x05;
    UART1->BRR1 = 0x04;

    UART1->CR1 = 0; // enable, 8bit, no parity
    UART1->CR2 = UART1_CR2_TEN|UART1_CR2_REN; // rx+tx
    UART1->CR3 = 0; // 1 sb, no clk
    UART1->CR4 = 0; // no lin, no addr
    UART1->CR5 = 0; // no smart card, no half duplex, no irda



    UART1->SR = (uint8_t)~(UART1_SR_RXNE);
    UART1->CR2 |= UART1_CR2_RIEN; // rx int en
}