/*
 * CC1101.h
 *
 *  Created on: Jan 11, 2022
 *      Author: Sergey
 */

#ifndef CC1101_CC1101_H_
#define CC1101_CC1101_H_

#include <stdint.h>

/*
 * Initial SPI settings and CS prepare should be implemented here
 */
uint8_t CC1101_GPIO_Prepare();

/*
 * Check RF`s ID, write initial config, defined in CC1101_macro.h
 */
uint8_t CC1101_Init();

/* Set GDO0 mode */
uint8_t CC1101_Set_GDO0(uint8_t GDO0_mode);

/*
 * Transmitting of byte flow with 1 <= length <= 62
 * data - pointer to data array
 * size - quantity of bytes to be sent
 */
uint8_t CC1101_TransmitPacket(uint8_t* data, uint8_t size);

/*
 * Go to RX mode though t idle. In this mode tranceiver can receive packets
 */
uint8_t CC1101_GoToRX();

/*
 * Enter wakeup-on-radio mode with automatic packet detection
 */
uint8_t CC1101_GoToWOR();

/*
 * Check RX FIFO state.
 * return value - number of bytes in RX FIFO
 */
uint8_t CC1101_IsDataAvailable();

/*
 * Read packet from RX FIFO
 * data - pointer to data array
 * RSSI - pointer to instant RSSI
 * LQI - pointer to instant LQI
 * return value - number of received bytes in RX FIFO
 */
uint8_t CC1101_ReadPacket(uint8_t* data, uint8_t* RSSI, uint8_t* LQI);

/*
 * Read status register
 * return value - status register
 */
uint8_t CC1101_ReadStatus();

/*
 * Go to sleep CMD
 */
uint8_t CC1101_ToSleep();
/* set settings functions */

/* Set Base Frequency
 * mhz - freq in MHz
 */
void CC1101_SetBaseFreq(float mhz);

/* Set synq word
 * synqword - 16-bit synq word
 */
void CC1101_SetSynqWord(uint16_t synqword);

/* Set channel
 * channel - number of channel
 */
void CC1101_SetChannel(uint8_t channel);

/* Set modulation
 * modulation - refer to macro.h file to choose correct value
 */
void CC1101_SetModulation(uint8_t modulation);

/* Set attenuation in RX mode
 * value - refer to macro.h file to choose correct value
 */
void CC1101_SetAttenuator(uint8_t value);

/* Set transmit power
 * txPower - refer to macro.h file to choose correct value
 */
void CC1101_SetTXPower(uint8_t txPower);

/* Set addressation mode
 * addressationMode - refer to macro.h file to choose correct value
 * devAddr - device address(8 bit)
 * txAddr - address mark in packet(8 bit)
 */
void CC1101_SetAddressation(uint8_t addressationMode, uint8_t devAddr, uint8_t txAddr);

/* Set Autoflash mode
 * mode - refer to macro.h file to choose correct value
 */
void CC1101_SetAutoFlashRX(uint8_t mode);

/* Set additional mark status
 * mode - refer to macro.h file to choose correct value
 */
void CC1101_SetAddStatus(uint8_t mode);

/* Set data rate
 * datarate - refer to macro.h file to choose correct value
 */
void CC1101_SetDataRate(uint16_t datarate);

/* Set packet length mode
 * mode - refer to macro.h file to choose correct value
 */
void CC1101_SetPacketLengthMode(uint8_t mode);

/* Set preambule minimal size
 * preamb - refer to macro.h file to choose correct value
 */
void CC1101_SetPreambuleMinSize(uint8_t preamb);

/* Set CRC mode
 * CRCmode - refer to macro.h file to choose correct value
 */
void CC1101_SetCRCmode(uint8_t CRCmode);

/* Set data whitening
 * whitening - refer to macro.h file to choose correct value
 */
void CC1101_SetWhitening(uint8_t whitening);

/* Set forward error correction
 * mode - refer to macro.h file to choose correct value
 * only available in fixed-size packet mode
 */
void CC1101_SetFEC(uint8_t mode);

/* Set preambule quality indicator treshold
 * mode - refer to macro.h file to choose correct value
 */
void CC1101_SetPQI(uint8_t PQI);

/* Set carrier sense threshold */
void CC1101_SetCS_Thr(uint8_t thr);

/* Set deviation(1/2 of TX_bw)
 * d - deviation in KHz
 */
void CC1101_setDeviation(float d);

#define CC1101_OK		0
#define CC1101_ERROR	1

/* Set settings`s defines */
#define CC1101_GDO0_HZ		0x2E
#define CC1101_GDO0_RDY		0x29
#define CC1101_GDO0_CS		0x0E
#define CC1101_GDO0_LOSCOUT	0x27
#define CC1101_GDO0_CHNCLR	0x09

#define CC1101_MODUL_2FSK 	0
#define CC1101_MODUL_GFSK 	1
#define CC1101_MODUL_ASKOOK	3
#define CC1101_MODUL_4FSK 	4
#define CC1101_MODUL_MSK 	7

#define CC1101_ATTEN_0DBM	0
#define CC1101_ATTEN_6DBM	1
#define CC1101_ATTEN_12DBM	2
#define CC1101_ATTEN_18DBM	3

#define CC1101_TXPWR_10DBM	0xC0
#define CC1101_TXPWR_7DBM	0xC8
#define CC1101_TXPWR_5DBM	0x84
#define CC1101_TXPWR_0DBM	0x60
#define CC1101_TXPWR_M10DBM	0x34
#define CC1101_TXPWR_M15DBM	0x1D
#define CC1101_TXPWR_M20DBM	0x0E
#define CC1101_TXPWR_M30DBM	0x12

#define CC1101_ADDRM_NOADDR	0
#define CC1101_ADDRM_ADDRCH	1
#define CC1101_ADDRM_ADDRSB	2
#define CC1101_ADDRM_ADDRDB	3

#define CC1101_AUTOFLUSH_ON		1
#define CC1101_AUTOFLUSH_OFF 	0

#define CC1101_FIXED_PKTLN 	0b00
#define CC1101_DYNAM_PKTLN 	0b01

#define CC1101_DRATE_1200	0x0583
#define CC1101_DRATE_2400	0x0683
#define CC1101_DRATE_10K	0x0893
#define CC1101_DRATE_38K4	0x0A83
#define CC1101_DRATE_76K8	0x0B83
#define CC1101_DRATE_100K	0x08F8
#define CC1101_DRATE_250K	0x0D3B

#define CC1101_PREAMB_2B	0
#define CC1101_PREAMB_3B	1
#define CC1101_PREAMB_4B	2
#define CC1101_PREAMB_6B	3
#define CC1101_PREAMB_8B	4
#define CC1101_PREAMB_12B	5
#define CC1101_PREAMB_16B	6
#define CC1101_PREAMB_24B	7

#define CC1101_EN_CRC		1
#define CC1101_NO_CRC		0

#define CC1101_EN_WHITENING	1
#define CC1101_NO_WHITENING	0

#define CC1101_EN_FEC		1
#define CC1101_NO_FEC		0

#endif /* CC1101_CC1101_H_ */
