#ifndef _MAIN_H_
#define _MAIN_H_

#define PORT_LED    GPIOB
#define PIN_LED     GPIO_PIN_5

#define PORT_NRF    GPIOC
#define PIN_NRF_CS  GPIO_PIN_4
#define PIN_NRF_CE  GPIO_PIN_3

/*
 * PD5 - UART1_TX
 * PD6 - UART1_RX
 *
 * PC7 - SPI_MISO
 * PC6 - SPI_MOSI
 * PC5 - SPI_SCK
 *
 * */

typedef struct {
    uint8_t * rxbufptr; // store received from RF data here
    uint8_t rxbufsize;  // received data size
    uint8_t * txbufptr; // data ready for transmittion
    uint8_t txbufsize;  // ready data size
} nrfctl_t;


#endif
