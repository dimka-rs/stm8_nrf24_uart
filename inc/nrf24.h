#ifndef _NRF_H_
#define _NRF_H_

/* various constants */
#define MAX_FRAME_SIZE 32	//max tx payload size in bytes
#define CE_MIN_HIGH 15		//minimal CE high time in ms
#define REG_WMASK 0x20
#define ADDR_SIZE 5

/* registers */
#define NRF24_CONFIG 0x00
enum reg_config_t {
    CONFIG_MASK_RX_DR =     (1 << 6),
    CONFIG_MASK_TX_DS =     (1 << 5),
    CONFIG_MASK_MAX_RT =    (1 << 4),
    CONFIG_EN_CRC =         (1 << 3),
    CONFIG_CRCO =           (1 << 2),
    CONFIG_PWR_UP =         (1 << 1),
    CONFIG_PRIM_RX =        (1 << 0),
};

#define NRF24_EN_AA 0x01
enum reg_enaa_t {
    ENAA_P5 =   (1 << 5),
    ENAA_P4 =   (1 << 4),
    ENAA_P3 =   (1 << 3),
    ENAA_P2 =   (1 << 2),
    ENAA_P1 =   (1 << 1),
    ENAA_P0 =   (1 << 0),
};

#define NRF24_EN_RXADDR 0x02
enum reg_enrxaddr {
    ENRXA_P5 = (1 << 5),
    ENRXA_P4 = (1 << 4),
    ENRXA_P3 = (1 << 3),
    ENRXA_P2 = (1 << 2),
    ENRXA_P1 = (1 << 1),
    ENRXA_P0 = (1 << 0),
};

#define NRF24_SETUP_AW 0x03
enum reg_setupaw {
    SETUP_AW_3 = 1,
    SETUP_AW_4 = 2,
    SETUP_AW_5 = 3,
};

#define NRF24_SETUP_RETR 0x04
enum reg_setupretr {
    SETUP_RETR_ARD_250 =    (0 << 4),
    SETUP_RETR_ARD_500 =    (1 << 4),
    SETUP_RETR_ARD_750 =    (2 << 4),
    SETUP_RETR_ARD_1000 =   (3 << 4),
    SETUP_RETR_ARD_1250 =   (4 << 4),
    SETUP_RETR_ARD_1500 =   (5 << 4),
    SETUP_RETR_ARD_1750 =   (6 << 4),
    SETUP_RETR_ARD_2000 =   (7 << 4),
    SETUP_RETR_ARD_2250 =   (8 << 4),
    SETUP_RETR_ARD_2500 =   (9 << 4),
    SETUP_RETR_ARD_2750 =   (10 << 4),
    SETUP_RETR_ARD_3000 =   (11 << 4),
    SETUP_RETR_ARD_3250 =   (12 << 4),
    SETUP_RETR_ARD_3500 =   (13 << 4),
    SETUP_RETR_ARD_3750 =   (14 << 4),
    SETUP_RETR_ARD_4000 =   (15 << 4),
    SETUP_RETR_ARC_0 =      (0),
    SETUP_RETR_ARC_1 =      (1),
    SETUP_RETR_ARC_2 =      (2),
    SETUP_RETR_ARC_3 =      (3),
    SETUP_RETR_ARC_4 =      (4),
    SETUP_RETR_ARC_5 =      (5),
    SETUP_RETR_ARC_6 =      (6),
    SETUP_RETR_ARC_7 =      (7),
    SETUP_RETR_ARC_8 =      (8),
    SETUP_RETR_ARC_9 =      (9),
    SETUP_RETR_ARC_10 =     (10),
    SETUP_RETR_ARC_11 =     (11),
    SETUP_RETR_ARC_12 =     (12),
    SETUP_RETR_ARC_13 =     (13),
    SETUP_RETR_ARC_14 =     (14),
    SETUP_RETR_ARC_15 =     (15),
};

#define NRF24_RF_CH 0x05
// channels are 0 to 63

#define NRF24_RF_SETUP 0x06
enum reg_rfsetup {
    RF_SETUP_CONT_WAVE =    (1 << 7),
    RF_SETUP_PLL_LOCK =     (1 << 4),
    RF_SETUP_DR_1M =        (0 << 5)|(0 << 3),
    RF_SETUP_DR_2M =        (0 << 5)|(1 << 3),
    RF_SETUP_DR_250K =      (1 << 5)|(0 << 3),
    RF_SETUP_PWR_M18DBM =   0,
    RF_SETUP_PWR_M12DBM =   2,
    RF_SETUP_PWR_M6DBM =    4,
    RF_SETUP_PWR_M0DBM =    6,
};

#define NRF24_STATUS 0x07
enum reg_status {
    STATUS_RX_DR =      (1 << 6),
    STATUS_TX_DS =      (1 << 5),
    STATUS_MAX_RT =     (1 << 4),
    STATUS_TX_FULL =    (1 << 0),
    STATUS_RX_P_MASK =  0x0E,
    STATUS_RX_P0 =      (0 << 1),
    STATUS_RX_P1 =      (1 << 1),
    STATUS_RX_P2 =      (2 << 1),
    STATUS_RX_P3 =      (3 << 1),
    STATUS_RX_P4 =      (4 << 1),
    STATUS_RX_P5 =      (5 << 1),
};

#define NRF24_OBSERVE_TX 0x08
enum reg_observetx {
    OBSERVE_TX_PLOS_CNT_MASK = 0xF0,
    OBSERVE_TX_ARC_CNT_MASK = 0x0F,
};

#define NRF24_RPD 0x09

#define NRF24_RX_ADDR_P0 0x0A // 5 bytes!
#define NRF24_RX_ADDR_P1 0x0B // 5 bytes!
#define NRF24_RX_ADDR_P2 0x0C
#define NRF24_RX_ADDR_P3 0x0D
#define NRF24_RX_ADDR_P4 0x0E
#define NRF24_RX_ADDR_P5 0x0F
#define NRF24_TX_ADDR 0x10 // 5 bytes!
#define NRF24_RX_PW_P0 0x11
#define NRF24_RX_PW_P1 0x12
#define NRF24_RX_PW_P2 0x13
#define NRF24_RX_PW_P3 0x14
#define NRF24_RX_PW_P4 0x15
#define NRF24_RX_PW_P5 0x16
#define NRF24_FIFO_STATUS 0x17
enum reg_fifostatus {
    FIFO_STATUS_TX_REUSE =  (1 << 6),
    FIFO_STATUS_TX_FULL =   (1 << 5),
    FIFO_STATUS_TX_EMPTY =  (1 << 4),
    FIFO_STATUS_RX_FULL =   (1 << 1),
    FIFO_STATUS_RX_EMPTY =  (1 << 0),
};

#define NRF24_DYNPD 0x1C
enum reg_dynpd {
    DYNPD_P5 =  (1 << 5),
    DYNPD_P4 =  (1 << 4),
    DYNPD_P3 =  (1 << 3),
    DYNPD_P2 =  (1 << 2),
    DYNPD_P1 =  (1 << 1),
    DYNPD_P0 =  (1 << 0),
};

#define NRF24_FEATURE 0x1D
enum reg_feature {
    FEATURE_EN_DPL =        (1 << 2),
    FEATURE_EN_ACK_PAY =    (1 << 1),
    FEATURE_EN_DYN_ACK =    (1 << 0),
};

/* commands */
#define NRF24_R_RX_PL_WID  0x60
#define NRF24_R_RX_PAYLOAD 0x61 //up to 32 bytes
#define NRF24_W_TX_PAYLOAD 0xA0 //up to 32 bytes
#define NRF24_FLUSH_TX 0xE1
#define NRF24_FLUSH_RX 0xE2
#define NRF24_REUSE_TX_PL  0xE3
#define NRF24_W_ACK_PAYLOAD 0xA8 //add 3 LSB pipe address
#define NRF24_W_TX_PAYLOAD_NOACK 0xB0

/* public functions */
uint8_t NRF_ReadReg(uint8_t regaddr);
void NRF_WriteReg(uint8_t regaddr, uint8_t regvalue);
void NRF_SendData(uint8_t * data, uint8_t size);
void NRF_WriteAddress(uint8_t regaddr, uint8_t * data, uint8_t size);
void NRF_WritePayload(uint8_t * data, uint8_t size);
void NRF_ReadPayload(uint8_t * data, uint8_t size);
void NRF_FlushTx();

#endif

