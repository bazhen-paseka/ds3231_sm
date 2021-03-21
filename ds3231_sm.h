/**
* \file
* \version 1.0
* \author bazhen.levkovets
** \date 2018
*
*************************************************************************************
* \copyright	Bazhen Levkovets
* \copyright	Brovary, Kyiv region
* \copyright	Ukraine
*
*************************************************************************************
*
* \brief
*
*/

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

	#include "ds3231_local_config.h"
	#include "i2c_techmaker_sm.h"
/*
**************************************************************************
*								    DEFINES
**************************************************************************
*/

/*
**************************************************************************
*                              FUNCTION PROTOTYPES
**************************************************************************
*/

void Set_Date_and_Time_by_str    (RTC_DateTypeDef * _dateSt, RTC_TimeTypeDef * _timeSt) ;
void Set_Day_and_Time_to_DS3231 (uint16_t _year_u16, uint8_t _month_u8, uint8_t _date_u8, uint8_t _hours_u8, uint8_t _minutes_u8, uint8_t _seconds_u8) ;

void ds3231_GetTime (uint8_t _ds3231_i2c_adr, RTC_TimeTypeDef * _timeSt)	;
void ds3231_GetDate (uint8_t _ds3231_i2c_adr, RTC_DateTypeDef * _dateSt)	;

void ds3231_PrintTime (RTC_TimeTypeDef * _timeSt, UART_HandleTypeDef *_huart)		;
void ds3231_PrintDate (RTC_DateTypeDef * _dateSt, UART_HandleTypeDef *_huart)		;
void ds3231_PrintWeek (RTC_DateTypeDef * _dateSt, UART_HandleTypeDef *_huart)		;
void ds3231_PrintWeek3char(RTC_DateTypeDef * _dateSt, UART_HandleTypeDef *_huart) 	;


void ds3231_Alarm1_SetEverySeconds		(uint8_t _ds3231_i2c_adr)									;
void ds3231_Alarm1_SetSeconds			(uint8_t _ds3231_i2c_adr, uint8_t _second)					;
void ds3231_Alarm1_SetHoursAndMinuses	(uint8_t _ds3231_i2c_adr, uint8_t _hours, uint8_t _minutes)	;

void ds3231_Alarm2_SetEveryMinutes		(uint8_t _ds3231_i2c_adr) 									;
void ds3231_Alarm2_SetMinuses			(uint8_t _ds3231_i2c_adr, uint8_t _minutes)					;
void ds3231_Alarm2_SetHoursAndMinuses	(uint8_t _ds3231_i2c_adr, uint8_t _hours, uint8_t _minutes)	;

void ds3231_Alarm1_Stop				(uint8_t _ds3231_i2c_adr)	;

void ds3231_Alarm1_ClearStatusBit	(uint8_t _ds3231_i2c_adr)	;
void ds3231_Alarm2_ClearStatusBit	(uint8_t _ds3231_i2c_adr)	;

uint8_t ds3231_Get_Alarm1_Status	(uint8_t _ds3231_i2c_adr)	;
uint8_t ds3231_Get_Alarm2_Status	(uint8_t _ds3231_i2c_adr)	;

void 	Ds3231_hard_alarm_flag_Set   	(void) ;
void 	Ds3231_hard_alarm_flag_Reset 	(void) ;
uint8_t Ds3231_hard_alarm_flag_Status	(void) ;

/*
**************************************************************************
*                                   END
**************************************************************************
*/

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
////
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

//	ABOUT RESET:

if (_rtc_isenabled != NULL) {
        if (!(_rtc_isenabled())) {
                                                                 //added the following three lines
                        if (_rtc_init != NULL) {
                                _rtc_init();
                            }
     //       set_time(0);// ORIG CODE HERE !!!!!!!!! commented out this will clear the RTC to Zero but alas not only here see below!
        }
    }
//------------	

if (_rtc_isenabled != NULL) {
        if (!(_rtc_isenabled())) {
                                                                 //added the following three lines
                        if (_rtc_init != NULL) {
                                _rtc_init();
                            }
     //       set_time(0);// ORIG CODE HERE !!!!!!!!! commented out this will clear the RTC to Zero but alas not only here see below!
        }
    }	
//------------	

    /* --- BDCR Register ---*/
/* Alias word address of RTCEN bit */
#define RCC_BDCR_OFFSET            (RCC_OFFSET + 0x70)
#define RCC_RTCEN_BIT_NUMBER       0x0F
#define RCC_BDCR_RTCEN_BB          (PERIPH_BB_BASE + (RCC_BDCR_OFFSET * 32) + (RCC_RTCEN_BIT_NUMBER * 4))
/* Alias word address of BDRST bit */
#define RCC_BDRST_BIT_NUMBER       0x10
#define RCC_BDCR_BDRST_BB          (PERIPH_BB_BASE + (RCC_BDCR_OFFSET * 32) + (RCC_BDRST_BIT_NUMBER * 4))
//------------	

void rtc_init(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    uint32_t rtc_freq = 0;
    
//add this code here:
//....................
 
    if(RTC->ISR != 7){ // Register Status Flag cold start value
        rtc_inited=1;
    }
//....................
        
    if (rtc_inited) return;
    rtc_inited = 1;
 
    RtcHandle.Instance = RTC;
The RSF is always '7' on a power up, then remains at '55' when the RTC has been set and on a reset. It may change to some other value depending on other RTC functions, but always seems to be '7' when MCU is powered up (without back up cell).

Need to make some more checks and refinements will probably be needed.	
//------------	
//------------	
//------------	