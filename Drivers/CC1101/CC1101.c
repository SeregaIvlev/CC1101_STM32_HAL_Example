#include "CC1101.h"
#include "CC1101_port.h"
#include "main.h"
#include "CC1101_macro.h"

uint8_t txAddr = 0;

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
	__CC1101_WriteCMD(CC1101_SIDLE);
	if((__CC1101_ReadReg(PKTCTRL1) & 0b11) != 0b00)
		size++;
	/* Length select */
	uint8_t pktformat = __CC1101_ReadReg(PKTCTRL0) & 0b11;
	if(pktformat == CC1101_DYNAM_PKTLN)
		__CC1101_WriteReg(CC1101_TXFIFO, size);
	if(pktformat == CC1101_FIXED_PKTLN)
		__CC1101_WriteReg(PKTLEN, size);

	/* Address select */
	if((__CC1101_ReadReg(PKTCTRL1) & 0b11) != 0b00)
		__CC1101_WriteReg(CC1101_TXFIFO, txAddr);


	/* Transmitting */
	__CC1101_BurstWriteReg(CC1101_TXFIFO, size, data);

	__CC1101_WriteCMD(CC1101_STX);
	while((__CC1101_ReadStatusRegs(CC1101_TXBYTES) & 0x7F) != 0);
	__CC1101_WriteCMD(CC1101_SFTX);
	CC1101_TXPacketCmpl_Callback();
	return CC1101_OK;
}
uint8_t CC1101_GoToRX()
{
	__CC1101_WriteCMD(CC1101_SIDLE);
	__CC1101_WriteCMD(CC1101_SRX);        //start receive
	return CC1101_OK;
}
uint8_t CC1101_IsDataAvailable()
{
	return __CC1101_ReadStatusRegs(CC1101_RXBYTES) & 0x7F;

}
uint8_t CC1101_ReadPacket(uint8_t* data, uint8_t* RSSI, uint8_t* LQI)
{


	uint8_t size;
	uint8_t status[2];

	if(__CC1101_ReadStatusRegs(CC1101_RXBYTES) & 0x7F)
	{
		size=__CC1101_ReadReg(CC1101_RXFIFO);
		__CC1101_BurstReadReg(CC1101_RXFIFO, size, data);
		__CC1101_BurstReadReg(CC1101_RXFIFO, 2, status);
		__CC1101_WriteCMD(CC1101_SFRX);
		__CC1101_WriteCMD(CC1101_SRX);
		*RSSI = status[0];
		*LQI = status[1];
		return size;
	}
	else
	{
		__CC1101_WriteCMD(CC1101_SFRX);
		__CC1101_WriteCMD(CC1101_SRX);
 		return 0;
	}
}
uint8_t CC1101_ReadStatus()
{
	___CC1101_USER_CS_Low();
	uint8_t status_value = ___CC1101_USER_SPI_TxRx(0x3D);
	___CC1101_USER_CS_High();
	return status_value;
}
uint8_t CC1101_ToSleep()
{
	__CC1101_WriteCMD(CC1101_SIDLE);
	__CC1101_WriteCMD(CC1101_SPWD);
	return CC1101_OK;
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
void CC1101_SetAddressation(uint8_t addressationMode, uint8_t devAddr, uint8_t NewTxAddress){

	txAddr = NewTxAddress;

	uint8_t data = __CC1101_ReadReg(PKTCTRL1);
	data &= 0b11111100;
	data |= addressationMode;
	__CC1101_WriteReg(PKTCTRL1, data);


	__CC1101_WriteReg(ADDR, devAddr);
}
void CC1101_SetPacketLengthMode(uint8_t mode){
	if(mode == CC1101_FIXED_PKTLN)
	{
		uint8_t data = 0b11111100 & __CC1101_ReadReg(PKTCTRL0);
		data |= 0b00;
		__CC1101_WriteReg(PKTCTRL0, data);

	}
	if(mode == CC1101_DYNAM_PKTLN)
	{
		__CC1101_WriteReg(PKTLEN, 0xFF);
		uint8_t data = 0b11111100 & __CC1101_ReadReg(PKTCTRL0);
		data |= 0b01;
		__CC1101_WriteReg(PKTCTRL0, data);
	}

}
void CC1101_SetAutoFlashRX(uint8_t mode)
{
	uint8_t data = __CC1101_ReadReg(PKTCTRL1);
	data &= 0b11110111;
	data |= mode<<3;
	__CC1101_WriteReg(PKTCTRL1, data);
}
void CC1101_SetAddStatus(uint8_t mode)
{
	uint8_t data = __CC1101_ReadReg(PKTCTRL1);
	data &= 0b11111011;
	data |= mode<<2;
	__CC1101_WriteReg(PKTCTRL1, mode);
}
void CC1101_SetDataRate(uint16_t datarate){
	__CC1101_WriteReg(MDMCFG3, (uint8_t)datarate);
	uint8_t data = __CC1101_ReadReg(MDMCFG4) & 0b11110000;
	data |= (datarate>>8) & 0b1111;
	__CC1101_WriteReg(MDMCFG4, data);

}
void CC1101_SetPreambuleMinSize(uint8_t preamb)
{
	uint8_t data = __CC1101_ReadReg(MDMCFG1) & 0b10001111;
	data |= preamb << 4;
	__CC1101_WriteReg(MDMCFG1, data);
}
void CC1101_SetCRCmode(uint8_t CRCmode)
{
	uint8_t data = __CC1101_ReadReg(PKTCTRL0) & 0b11111011;
	data |= CRCmode << 2;
	__CC1101_WriteReg(PKTCTRL0, data);
}
void CC1101_SetWhitening(uint8_t whitening)
{
	uint8_t data = __CC1101_ReadReg(PKTCTRL0) & 0b10111111;
	data |= whitening << 6;
	__CC1101_WriteReg(PKTCTRL0, data);
}
void CC1101_SetFEC(uint8_t mode)
{
	uint8_t data = __CC1101_ReadReg(MDMCFG1) & 0b01111111;
	data |= mode << 7;
	__CC1101_WriteReg(MDMCFG1, data);
}
void CC1101_SetPQI(uint8_t PQI){
	uint8_t data = __CC1101_ReadReg(PKTCTRL1) & 0b00011111;
	data |= PQI << 5;
	__CC1101_WriteReg(PKTCTRL1, data);
}
void CC1101_setDeviation(float d){
	#warning test it
	float f = 1.586914;
	float v = 0.19836425;
	int c = 0;

	if (d > 380.859375) {d = 380.859375;}
	if (d < 1.586914) {d = 1.586914;}

	for (int i = 0; i<255; i++){
		f+=v;
		if (c==7){v*=2;c=-1;i+=8;}
		if (f>=d){c=i;i=255;}
		c++;
	}
	__CC1101_WriteReg(DEVIATN,c);
}
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
	//addr &= 0b00111111;
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
