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


uint8_t CC1101_GPIO_Prepare();
uint8_t CC1101_Init();
uint8_t CC1101_TransmitPacket(uint8_t* data, uint8_t size);
uint8_t CC1101_GoToRX();

uint8_t CC1101_IsDataAvailable();
uint8_t CC1101_ReadPacket(uint8_t* data, uint8_t* RSSI, uint8_t* LQI);
uint8_t CC1101_ReadStatus();


/* set settings functions */
void CC1101_SetBaseFreq(float mhz);
void CC1101_SetSynqWord(uint16_t synqword);
void CC1101_SetChannel(uint8_t channel);
void CC1101_SetModulation(uint8_t modulation);
void CC1101_SetAttenuator(uint8_t value);
void CC1101_SetTXPower(uint8_t txPower);
void CC1101_SetAddressation(uint8_t addressationMode, uint8_t devAddr, uint8_t txAddr);
void CC1101_SetAutoFlashRX(uint8_t mode);
void CC1101_SetAddStatus(uint8_t mode);
void CC1101_SetDataRate(uint16_t datarate);
void CC1101_SetPacketLengthMode(uint8_t mode);
void CC1101_SetPreambuleMinSize(uint8_t preamb);
void CC1101_SetCRCmode(uint8_t CRCmode);
void CC1101_SetWhitening(uint8_t whitening);
void CC1101_SetFEC(uint8_t mode);
void CC1101_setDeviation(float d);

#define CC1101_OK		0
#define CC1101_ERROR	1

#endif /* CC1101_CC1101_H_ */
