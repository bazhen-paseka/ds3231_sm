#include "stm32f1xx_hal.h"
#include "i2c_techmaker_sm.h"
#include "ds3231_sm.h"
#include <string.h>

//	#define ADR_I2C_DS3231 0x68

void ds3231_PrintTime(RTC_TimeTypeDef * _timeSt, UART_HandleTypeDef *_huart)
{
	char DataChar[100];

	sprintf(DataChar,"%02d:%02d:%02d ",_timeSt->Hours, _timeSt->Minutes, _timeSt->Seconds);
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);
}
//-------------------------------------------------------------------------------------------------------------

void ds3231_PrintDate(RTC_DateTypeDef * _dateSt, UART_HandleTypeDef *_huart)
{
	char DataChar[100];

	sprintf(DataChar,"%04d/%02d/%02d ",2000+ _dateSt->Year, _dateSt->Month, _dateSt->Date);
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);

	switch(_dateSt->WeekDay)
		{
		case  7: sprintf(DataChar,"Sunday"); 		break;
		case  1: sprintf(DataChar,"Monday");		break;
		case  2: sprintf(DataChar,"Tuesday"); 		break;
		case  3: sprintf(DataChar,"Wednesday");		break;
		case  4: sprintf(DataChar,"Thursday");		break;
		case  5: sprintf(DataChar,"Friday");		break;
		case  6: sprintf(DataChar,"Saturday");		break;
		default: sprintf(DataChar,"Out of day");	break;
		} // end switch Date.ST
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);

	sprintf(DataChar,"\r\n");
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);
}
//-------------------------------------------------------------------------------------------------------------

void ds3231_GetTime(uint8_t _ds3231_i2c_adr, RTC_TimeTypeDef * _timeSt)
{
	uint8_t ds3231_Seconds ;
	uint8_t ds3231_Minutes ;
	uint8_t ds3231_Hours   ;

	I2Cdev_readByte( _ds3231_i2c_adr, 0x00, &ds3231_Seconds, 100);
	I2Cdev_readByte( _ds3231_i2c_adr, 0x01, &ds3231_Minutes, 100);
	I2Cdev_readByte( _ds3231_i2c_adr, 0x02, &ds3231_Hours,   100);

	_timeSt->Hours   = (ds3231_Hours   >> 4)*10 + (ds3231_Hours   &0x0F);
	_timeSt->Minutes = (ds3231_Minutes >> 4)*10 + (ds3231_Minutes &0x0F);
	_timeSt->Seconds = (ds3231_Seconds >> 4)*10 + (ds3231_Seconds &0x0F);
}
//-------------------------------------------------------------------------------------------------------------

void ds3231_GetDate(uint8_t _ds3231_i2c_adr, RTC_DateTypeDef * _dateSt)
{
	uint8_t ds3231_WeekDay ;
	uint8_t ds3231_Date    ;
	uint8_t ds3231_Mouth   ;
	uint8_t ds3231_Year    ;

	I2Cdev_readByte( _ds3231_i2c_adr, 0x03, &ds3231_WeekDay, 100);
	I2Cdev_readByte( _ds3231_i2c_adr, 0x04, &ds3231_Date,    100);
	I2Cdev_readByte( _ds3231_i2c_adr, 0x05, &ds3231_Mouth,   100);
	I2Cdev_readByte( _ds3231_i2c_adr, 0x06, &ds3231_Year,    100);

	_dateSt->WeekDay = (ds3231_WeekDay >> 4)*10 + (ds3231_WeekDay &0x0F);
	_dateSt->Date    = (ds3231_Date    >> 4)*10 + (ds3231_Date    &0x0F);
	_dateSt->Month   = (ds3231_Mouth   >> 4)*10 + (ds3231_Mouth   &0x0F);
	_dateSt->Year    = (ds3231_Year    >> 4)*10 + (ds3231_Year    &0x0F);
}
//-------------------------------------------------------------------------------------------------------------

void ds3231_SetTime(uint8_t _ds3231_i2c_adr, RTC_TimeTypeDef * _timeSt)
{
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x00, _timeSt->Seconds );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x01, _timeSt->Minutes );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x02, _timeSt->Hours   );
}

void ds3231_SetDate(uint8_t _ds3231_i2c_adr, RTC_DateTypeDef * _dateSt)
{
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x03, _dateSt->WeekDay );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x04, _dateSt->Date    );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x05, _dateSt->Month   );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x06, _dateSt->Year    );
}


void ds3231_Alarm1_SetEverySeconds(uint8_t _ds3231_i2c_adr,UART_HandleTypeDef *_huart)
{
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x07, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x08, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x09, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x0A, 1UL<<7 );

	uint8_t alarm_status;
	I2Cdev_readByte ( _ds3231_i2c_adr, 0x0E, &alarm_status, 100   );

	char DataChar[100];
	sprintf(DataChar,"alarm_status %d\r\n", (int)alarm_status);
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);

	I2Cdev_writeByte( _ds3231_i2c_adr, 0x0E,  0b00000101 );

//	I2Cdev_writeByte( _ds3231_i2c_adr, 0x0E,  0b00011111 );
}


void ds3231_Alarm1_SetSeconds(uint8_t _ds3231_i2c_adr, uint8_t _second)
{
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x07, _second );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x08, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x09, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x0A, 1UL<<7 );

//	uint8_t alarm_status;
//	I2Cdev_readByte ( _ds3231_i2c_adr, 0x0E, &alarm_status, 100   );
//	I2Cdev_writeByte( _ds3231_i2c_adr, 0x0E,  alarm_status||(1UL<<2)||(1UL) );

	I2Cdev_writeByte( _ds3231_i2c_adr, 0x0E,  0b00000101 );

}

void ds3231_Alarm1_ReadStatusBit(uint8_t _ds3231_i2c_adr,UART_HandleTypeDef *_huart)
{
	uint8_t status_bit;
	I2Cdev_readByte ( _ds3231_i2c_adr, 0x0F, &status_bit, 100   );

	char DataChar[100];
	sprintf(DataChar,"status_bit %d\r\n", (int)status_bit);
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);
}

void ds3231_Alarm1_ClearStatusBit(uint8_t _ds3231_i2c_adr,UART_HandleTypeDef *_huart)
{
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x0F,  0b10001000 );

	uint8_t status_bit;
	I2Cdev_readByte ( _ds3231_i2c_adr, 0x0F, &status_bit, 100   );

	char DataChar[100];
	sprintf(DataChar,"_clear_bit %d\r\n", (int)status_bit);
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);
}

void ds3231_Alarm1_Stop(uint8_t _ds3231_i2c_adr)
{
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x07, 0 );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x08, 0 );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x09, 0 );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x0A, 0 );

	uint8_t alarm_status;
	I2Cdev_readByte ( _ds3231_i2c_adr, 0x0E, &alarm_status, 100   );
	//I2Cdev_writeByte( _ds3231_i2c_adr, 0x0E,  alarm_status&&(0UL) );
	I2Cdev_writeByte( _ds3231_i2c_adr, 0x0E,  0b00011111 );
}
