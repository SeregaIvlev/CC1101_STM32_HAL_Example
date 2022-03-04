/*
 * CC1101.h
 *
 *  Created on: Jan 11, 2022
 *      Author: Sergey
 */

#ifndef CC1101_CC1101_H_
#define CC1101_CC1101_H_

#include <stdint.h>


/* Default config:
 * Base Freq: 		433.0
 * Channel: 		0
 * Modulation: 		GFSK
 * TxPower: 		0 dBm
 * Data rate: 		20 kBaud
 * CRC: 			enabled
 * Channel Spacing: 199.5 kHz
 * RX filter BW: 	325 kHz
 * Deviation: 		47.6 kHz
 * Whitening:		Disabled
 * Manchester: 		Disabled
 * Mode: 			Variable length
 * SynqWord:		D3 91
 * Address check:	No address check
 * Preabule:		30/32 synq word detected
 * Attenuator: 		0 dBm
 * Address:			0x00
 */

/*
 * Initial SPI settings and CS prepare should be implemented here
 */
uint8_t CC1101_GPIO_Prepare();

/*
 * Check RF`s ID, write initial config, defined in CC1101_macro.h
 */
uint8_t CC1101_Init();

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
void CC1101_SetPQI(uint8_t PQI){
	uint8_t data = __CC1101_ReadReg(PKTCTRL1) & 0b00011111;
	data |= PQI << 5;
	__CC1101_WriteReg(PKTCTRL1, data);
}

/* Set deviation(1/2 of TX_bw)
 * d - deviation in KHz
 */
void CC1101_setDeviation(float d);

#define CC1101_OK		0
#define CC1101_ERROR	1

#endif /* CC1101_CC1101_H_ */
