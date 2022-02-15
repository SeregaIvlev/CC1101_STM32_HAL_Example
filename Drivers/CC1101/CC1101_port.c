#include "main.h"

#define CC_SPI hspi1
extern SPI_HandleTypeDef CC_SPI;

uint8_t ___CC1101_USER_SPI_TxRx(uint8_t dataTX){
	uint8_t dataRX = 0;
	HAL_SPI_TransmitReceive(&CC_SPI, &dataTX, &dataRX, 1, 100);
	return dataRX;
}
void ___CC1101_USER_CS_High(){
	HAL_GPIO_WritePin(CC1101_CS_GPIO_Port, CC1101_CS_Pin, SET);
}
void ___CC1101_USER_CS_Low(){
	HAL_GPIO_WritePin(CC1101_CS_GPIO_Port, CC1101_CS_Pin, RESET);
}


__weak void CC1101_RXPacketCmpl_Callback()
{
	nop();
}
__weak void CC1101_TXPacketCmpl_Callback()
{
	nop();
}
