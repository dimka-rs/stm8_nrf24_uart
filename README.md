# stm8 nrf24 uart

Simple bidirectional _half-dupex_ UART over NRF24.
Transmitting has precedence over receiving, so it will only work with request-response type of communication.

## Theory of operation:
* Device listens for incoming data from NRF ot UART
* If data is received from NFR, it is then sent over UART
* If data received from UART, timeout timer is started
  * If buffer if full, data is sent over NRF
  * if timeout reached, data is sent over NRF
