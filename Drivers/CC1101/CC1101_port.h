/*
 * CC1101_port.h
 *
 *  Created on: Jan 11, 2022
 *      Author: Sergey
 */

#ifndef CC1101_CC1101_PORT_H_
#define CC1101_CC1101_PORT_H_

#include <stdint.h>

uint8_t ___CC1101_USER_SPI_TxRx(uint8_t dataTX);
void ___CC1101_USER_CS_High();
void ___CC1101_USER_CS_Low();

void CC1101_RXPacketCmpl_Callback();
void CC1101_TXPacketCmpl_Callback();

#endif /* CC1101_CC1101_PORT_H_ */
