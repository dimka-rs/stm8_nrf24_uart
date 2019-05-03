#include "stm8s.h"
#include "main.h"

void InitUART()
{
    /* UART 1 */

    // UART_DIV = f MASTER / baud rate
    // UART_DIV = 16 * 10^6 / 115200
    // UART_DIV = 138.89 = 0x008B = 0x0 (BRR2_H), 0x08 (BRR1), 0x0B (BRR2_L)
    // prescaler updated after BRR1 write, so BRR2 is written first
    UART1->BRR2 = 0x0B;
    UART1->BRR1 = 0x08;


    UART1->CR1 = 0; // enable, 8bit, no parity
    UART1->CR2 = UART1_CR2_TEN|UART1_CR2_REN; // rx+tx
    UART1->CR3 = 0; // 1 sb, no clk
    UART1->CR4 = 0; // no lin, no addr
    UART1->CR5 = 0; // no smart card, no half duplex, no irda



    UART1->SR = (uint8_t)~(UART1_SR_RXNE);
    UART1->CR2 |= UART1_CR2_RIEN; // rx int en
}