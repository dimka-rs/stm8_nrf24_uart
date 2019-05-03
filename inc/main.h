#ifndef _MAIN_H_
#define _MAIN_H_

#define LED_PORT    GPIOB
#define LED_PIN     GPIO_PIN_5

#define NRF_PORT    GPIOC
#define NRF_CS_PIN  GPIO_PIN_4
#define NRF_CE_PIN  GPIO_PIN_3

#define NRF_IRQ_PORT    GPIOD
#define NRF_IRQ_PIN     GPIO_PIN_2

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
