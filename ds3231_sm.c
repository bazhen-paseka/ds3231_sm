#include "stm32f1xx_hal.h"
#include "i2c_techmaker_sm.h"
#include "ds3231_sm.h"
#include <string.h>


//	RTC_TimeTypeDef TimeSt;
//	RTC_DateTypeDef DateSt;

	#define ADR_I2C_DS3231 0x68

void ds3231_GetTime(ds3231_struct *_ds3221_handler, RTC_TimeTypeDef * _timeSt, RTC_DateTypeDef * _dateSt, UART_HandleTypeDef *_huart)
{
	char DataChar[100];

	uint8_t ds3231_Seconds = 0x05;
	uint8_t ds3231_Minutes = 0x26;
	uint8_t ds3231_Hours   = 0x16;
	uint8_t ds3231_WeekDay = 0x04;
	uint8_t ds3231_Date    = 0x05;
	uint8_t ds3231_Mouth   = 0x09;
	uint8_t ds3231_Year    = 0x19;

	sprintf(DataChar,"[in] start Get Time\r\n");
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);

//	I2Cdev_readByte( _ds3221_handler->i2c_adr, 0x00, &ds3231_Seconds, 100);
//	I2Cdev_readByte( _ds3221_handler->i2c_adr, 0x01, &ds3231_Minutes, 100);
//	I2Cdev_readByte( _ds3221_handler->i2c_adr, 0x02, &ds3231_Hours,   100);
//	I2Cdev_readByte( _ds3221_handler->i2c_adr, 0x03, &ds3231_WeekDay, 100);
//	I2Cdev_readByte( _ds3221_handler->i2c_adr, 0x04, &ds3231_Date,    100);
//	I2Cdev_readByte( _ds3221_handler->i2c_adr, 0x05, &ds3231_Mouth,   100);
//	I2Cdev_readByte( _ds3221_handler->i2c_adr, 0x06, &ds3231_Year,    100);

	I2Cdev_readByte( ADR_I2C_DS3231, 0x00, &ds3231_Seconds, 100);
	I2Cdev_readByte( ADR_I2C_DS3231, 0x01, &ds3231_Minutes, 100);
	I2Cdev_readByte( ADR_I2C_DS3231, 0x02, &ds3231_Hours,   100);
	I2Cdev_readByte( ADR_I2C_DS3231, 0x03, &ds3231_WeekDay, 100);
	I2Cdev_readByte( ADR_I2C_DS3231, 0x04, &ds3231_Date,    100);
	I2Cdev_readByte( ADR_I2C_DS3231, 0x05, &ds3231_Mouth,   100);
	I2Cdev_readByte( ADR_I2C_DS3231, 0x06, &ds3231_Year,    100);

	_timeSt->Hours   = (ds3231_Hours   >> 4)*10 + (ds3231_Hours   &0x0F);
	_timeSt->Minutes = (ds3231_Minutes >> 4)*10 + (ds3231_Minutes &0x0F);
	_timeSt->Seconds = (ds3231_Seconds >> 4)*10 + (ds3231_Seconds &0x0F);
	_dateSt->WeekDay = (ds3231_WeekDay >> 4)*10 + (ds3231_WeekDay &0x0F);
	_dateSt->Date    = (ds3231_Date    >> 4)*10 + (ds3231_Date    &0x0F);
	_dateSt->Month   = (ds3231_Mouth   >> 4)*10 + (ds3231_Mouth   &0x0F);
	_dateSt->Year    = (ds3231_Year    >> 4)*10 + (ds3231_Year    &0x0F);

	sprintf(DataChar,"[in] %02d:%02d:%02d ",_timeSt->Hours, _timeSt->Minutes, _timeSt->Seconds);
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);

	sprintf(DataChar,"[in] %04d/%02d/%02d\r\n",2000+ _dateSt->Year, _dateSt->Month, _dateSt->Date);
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);
}
