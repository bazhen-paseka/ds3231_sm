#ifndef DS3231_SM_H_INCLUDED
#define DS3231_SM_H_INCLUDED

/*
**************************************************************************
*								INCLUDE FILES
**************************************************************************
*/

	#include "main.h"
	#include <string.h>
	#include "stdio.h"
	#include "i2c_techmaker_sm.h"

/*
**************************************************************************
*								    DEFINES
**************************************************************************
*/

	#define ADR_I2C_DS3231 0x68

/*
**************************************************************************
*                              FUNCTION PROTOTYPES
**************************************************************************
*/

void Set_Date_and_Time_by_str    (RTC_DateTypeDef * _dateSt, RTC_TimeTypeDef * _timeSt);
void Set_Date_and_Time_to_DS3231 (uint8_t _year_u8, uint8_t _month_u8, uint8_t _date_u8, uint8_t _hours_u8, uint8_t _minutes_u8, uint8_t _seconds_u8);

void ds3231_GetTime (uint8_t _ds3231_i2c_adr, RTC_TimeTypeDef * _timeSt);
void ds3231_GetDate (uint8_t _ds3231_i2c_adr, RTC_DateTypeDef * _dateSt);

void ds3231_PrintTime (RTC_TimeTypeDef * _timeSt, UART_HandleTypeDef *_huart);
void ds3231_PrintDate (RTC_DateTypeDef * _dateSt, UART_HandleTypeDef *_huart);
void ds3231_PrintWeek (RTC_DateTypeDef * _dateSt, UART_HandleTypeDef *_huart);

void ds3231_Alarm1_SetSeconds      (uint8_t _ds3231_i2c_adr, uint8_t _second);
void ds3231_Alarm1_SetEverySeconds (uint8_t _ds3231_i2c_adr);

void ds3231_Alarm1_Stop (uint8_t _ds3231_i2c_adr);

void ds3231_Alarm1_ClearStatusBit (uint8_t _ds3231_i2c_adr);
void ds3231_Alarm2_ClearStatusBit (uint8_t _ds3231_i2c_adr);

#endif // DS3231_SM_H_INCLUDED


//	Number 123
//	Binary Form 01111011
//	BCD will be 0001 0010 0011


// 			SET TIME and DATE
//
//	#include <string.h>
//	#include "i2c_techmaker_sm.h"
//	#include "ds3231_sm.h"
//
//	#define ADR_I2C_DS3231 0x68
//
//	RTC_TimeTypeDef TimeSt;
//	RTC_DateTypeDef DateSt;

//	I2Cdev_init(&hi2c1);
//	I2C_ScanBusFlow(&hi2c1, &huart1);
//
//	TimeSt.Seconds = 0x03 ;
//	TimeSt.Minutes = 0x40 ;
//	TimeSt.Hours   = 0x14 ;
//	ds3231_SetTime(ADR_I2C_DS3231, &TimeSt);
//
//	DateSt.Date  = 0x06 ;
//	DateSt.Month = 0x09 ;
//	DateSt.Year  = 0x19 ;
//	ds3231_SetDate(ADR_I2C_DS3231, &DateSt);
//
//	ds3231_PrintTime(ADR_I2C_DS3231, &TimeSt, &huart1);
//	ds3231_PrintDate(ADR_I2C_DS3231, &DateSt, &huart1);
//
// 				END


//			READ TIME and DATE from DS3231 and write to RTC
//
//	#include <string.h>
//	#include "i2c_techmaker_sm.h"
//	#include "ds3231_sm.h"
//
//	#define ADR_I2C_DS3231 0x68
//
//	RTC_TimeTypeDef TimeSt;
//	RTC_DateTypeDef DateSt;
//
//	I2Cdev_init(&hi2c1);
//	I2C_ScanBusFlow(&hi2c1, &huart1);
//
//	ds3231_GetTime(ADR_I2C_DS3231, &TimeSt);
//	ds3231_GetDate(ADR_I2C_DS3231, &DateSt);
//
//	HAL_RTC_SetTime( &hrtc, &TimeSt, RTC_FORMAT_BIN );
//	HAL_RTC_SetDate( &hrtc, &DateSt, RTC_FORMAT_BIN );
//
//	HAL_RTC_GetTime( &hrtc, &TimeSt, RTC_FORMAT_BIN );
//	HAL_RTC_GetDate( &hrtc, &DateSt, RTC_FORMAT_BIN );
//
//	ds3231_PrintTime(&TimeSt, &huart1);
//	ds3231_PrintDate(&DateSt, &huart1);
//
// 				END
