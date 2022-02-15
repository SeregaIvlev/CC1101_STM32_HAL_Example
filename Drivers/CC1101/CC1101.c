#include "CC1101.h"
#include "CC1101_port.h"
#include "CC1101_macro.h"

#include "main.h"

uint8_t __CC1101_ReadReg(uint8_t addr);
void __CC1101_WriteReg(uint8_t addr, uint8_t data);
void __CC1101_BurstReadReg(uint8_t addr, uint8_t lth, uint8_t* data);
void __CC1101_BurstWriteReg(uint8_t addr, uint8_t lth, uint8_t* data);
void __CC1101_WriteCMD(uint8_t cmd);
uint8_t __CC1101_ReadStatusRegs(uint8_t addr);


uint8_t CC1101_GPIO_Prepare(){
	___CC1101_USER_CS_High();
	return CC1101_OK;
}
uint8_t CC1101_Init(){

	__CC1101_WriteCMD(CC1101_SRES);
	HAL_Delay(100);
	uint8_t ID = __CC1101_ReadReg(AGCTEST);
	if(ID != CC1101_DEVICE_ID)
		return CC1101_ERROR;

	__CC1101_BurstWriteReg(0x00, 47, CC1101_DefaultSettings);

	return CC1101_OK;
}
uint8_t CC1101_TransmitPacket(uint8_t* data, uint8_t size){
	__CC1101_WriteReg(CC1101_TXFIFO, size);
	__CC1101_BurstWriteReg(CC1101_TXFIFO, size, data);
	__CC1101_WriteCMD(CC1101_SIDLE);
	__CC1101_WriteCMD(CC1101_STX);
	while((__CC1101_ReadStatusRegs(CC1101_TXBYTES) & 0x7F)  != 0);
	__CC1101_WriteCMD(CC1101_SFTX);
	__CC1101_WriteCMD(CC1101_SIDLE);
	CC1101_TXPacketCmpl_Callback();
	return CC1101_OK;
}
uint8_t CC1101_IsDataAvalable()
{
	return CC1101_OK;
}
uint8_t CC1101_ReadPacket()
{
	return CC1101_OK;
}
uint8_t CC1101_ReadStatus()
{
	___CC1101_USER_CS_Low();
	uint8_t status_value = ___CC1101_USER_SPI_TxRx(0x3D);
	___CC1101_USER_CS_High();
	return status_value;
}

/* set settings functions */
void CC1101_SetBaseFreq(float mhz)
{
	uint8_t freq2 = 0;
	uint8_t freq1 = 0;
	uint8_t freq0 = 0;


	for (uint8_t i = 0; i==0;){
	if (mhz >= 26){
	mhz-=26;
	freq2+=1;
	}
	else if (mhz >= 0.1015625){
	mhz-=0.1015625;
	freq1+=1;
	}
	else if (mhz >= 0.00039675){
	mhz-=0.00039675;
	freq0+=1;
	}
	else{i=1;}
	}
	if (freq0 > 255){freq1+=1;freq0-=256;}

	__CC1101_WriteReg(FREQ2, freq2);
	__CC1101_WriteReg(FREQ1, freq1);
	__CC1101_WriteReg(FREQ0, freq0);

	//Calibrate();
}
void CC1101_SetSynqWord(uint16_t synqword)
{
	__CC1101_WriteReg(SYNC1, synqword>>8);
	__CC1101_WriteReg(SYNC0, (uint8_t)synqword);
}
void CC1101_SetChannel(uint8_t channel){
	__CC1101_WriteReg(CHANNR, channel);
}
void CC1101_SetModulation(uint8_t modulation){
	uint8_t data = __CC1101_ReadReg(MDMCFG2);
	data &= 0b10001111;
	data |= modulation<<4;
	__CC1101_WriteReg(MDMCFG2, data);
}
void CC1101_SetAttenuator(uint8_t value){
	uint8_t data = __CC1101_ReadReg(FIFOTHR);
	data &= 0b11001111;
	data |= value<<4;
	__CC1101_WriteReg(FIFOTHR, data);
}
void CC1101_SetTXPower(uint8_t txPower){
	#warning not tested
	__CC1101_BurstWriteReg(CC1101_PATABLE, 1, &txPower);
}
void CC1101_SetAddressation(uint8_t addressationMode, uint8_t devAddr){

	#warning not tested
	uint8_t data = __CC1101_ReadReg(PKTCTRL1);
	data &= 0b11111100;
	data |= addressationMode;
	__CC1101_WriteReg(PKTCTRL1, data);


	__CC1101_WriteReg(ADDR, devAddr);
}
void CC1101_SetAutoFlashRX(uint8_t mode)
{
	#warning not tested
	uint8_t data = __CC1101_ReadReg(PKTCTRL1);
	data &= 0b11110111;
	data |= mode<<3;
	__CC1101_WriteReg(PKTCTRL1, data);
}
void CC1101_SetAddStatus(uint8_t mode)
{
	#warning not tested
	uint8_t data = __CC1101_ReadReg(PKTCTRL1);
	data &= 0b11111011;
	data |= mode<<2;
	__CC1101_WriteReg(PKTCTRL1, mode);
}

void CC1101_SetDataRate(float datarate);

/* non-user functions */
uint8_t __CC1101_ReadReg(uint8_t addr)
{
	uint8_t data = 0;
	addr &= 0b00111111;
	___CC1101_USER_CS_Low();
	___CC1101_USER_SPI_TxRx((1<<7) | addr);
	data = ___CC1101_USER_SPI_TxRx(0x00);
	___CC1101_USER_CS_High();
	return data;
}
void __CC1101_WriteReg(uint8_t addr, uint8_t data)
{
	addr &= 0b00111111;
	___CC1101_USER_CS_Low();
	___CC1101_USER_SPI_TxRx(addr);
	___CC1101_USER_SPI_TxRx(data);
	___CC1101_USER_CS_High();
}
void __CC1101_BurstReadReg(uint8_t addr, uint8_t lth, uint8_t* data)
{
	addr &= 0b00111111;
	___CC1101_USER_CS_Low();
	___CC1101_USER_SPI_TxRx( (1<<7) | (1<<6) | addr); //Read and burst bits

	for(uint8_t n = 0; n < lth; n++)
	{
		data[n] = ___CC1101_USER_SPI_TxRx(0x00);
	}

	___CC1101_USER_CS_High();
}
void __CC1101_BurstWriteReg(uint8_t addr, uint8_t lth, uint8_t* data)
{
	addr &= 0b00111111;
	___CC1101_USER_CS_Low();
	___CC1101_USER_SPI_TxRx( (1<<6) | addr);		//Write and burst bits

	for(uint8_t n = 0; n < lth; n++)
	{
		___CC1101_USER_SPI_TxRx(data[n]);
	}

	___CC1101_USER_CS_High();
}
void __CC1101_WriteCMD(uint8_t cmd)
{
	___CC1101_USER_CS_Low();
	___CC1101_USER_SPI_TxRx(cmd);
	___CC1101_USER_CS_High();

}
uint8_t __CC1101_ReadStatusRegs(uint8_t addr)
{
  uint8_t value,temp;
  temp = addr | (1<<7) | (1<<6);
  ___CC1101_USER_CS_Low();
  ___CC1101_USER_SPI_TxRx(temp);
  value= ___CC1101_USER_SPI_TxRx(0);
  ___CC1101_USER_CS_High();
  return value;
}
