
/*
**************************************************************************
*								INCLUDE FILES
**************************************************************************
*/

	#include "ds3231_sm.h"

/*
**************************************************************************
*								   DATA TYPES
**************************************************************************
*/

/*
**************************************************************************
*								    ENUM
**************************************************************************
*/
typedef enum {
	DS3231_SECONDS	= 0x00		,
	DS3231_MINUTES				,
	DS3231_HOUR					,
	DS3231_WEEKDAY				,
	DS3231_DATE					,
	DS3231_MONTH				,
	DS3231_YEAR					,
	DS3231_ALARM_1_SECONDS		,
	DS3231_ALARM_1_MINUTES		,
	DS3231_ALARM_1_HOUR			,
	DS3231_ALARM_1_DAY_AND_DATE	,
	DS3231_ALARM_2_MINUTES		,
	DS3231_ALARM_2_HOUR			,
	DS3231_ALARM_2_DAY_AND_DATE	,
	DS3231_CONTROL				,
	DS3231_CONTROL_STATUS		,
	DS3231_AGING_OFFSET			,
	DS3231_MSB_OF_TEMP			,
	DS3231_lSB_OF_TEMP
}		ds3231_register ;
//=============================================

typedef enum {
	DS3231_CONTROL_A1IE	=	0x00,
	DS3231_CONTROL_A2IE			,
	DS3231_CONTROL_INTCN		,
	DS3231_CONTROL_RS1			,
	DS3231_CONTROL_RS2			,
	DS3231_CONTROL_CONV			,
	DS3231_CONTROL_BBSQW		,
	DS3231_CONTROL_EOSC
}		ds3231_register_status ;
//=============================================

typedef enum {
	DS3231_CNTRL_STATUS_A1F	= 0x00	,
	DS3231_CNTRL_STATUS_A2F			,
	DS3231_CNTRL_STATUS_BSY			,
	DS3231_CNTRL_STATUS_EN32KHZ		,
	DS3231_CNTRL_STATUS_EMPTY_4		,
	DS3231_CNTRL_STATUS_EMPTY_5		,
	DS3231_CNTRL_STATUS_EMPTY_6		,
	DS3231_CNTRL_STATUS_OSF
}		ds3231_register_cntrl_status ;
//=============================================

volatile uint8_t ds3231_alarm_u8 = 0 ;

/*
**************************************************************************
*								    DEFINES
**************************************************************************
*/

/*
**************************************************************************
*   							   MACRO'S
**************************************************************************
*/
	#define BIT_SET(   byte , pos )		((byte) |=  (1UL << (pos)))
	#define BIT_CLR(   byte , pos )		((byte) &= ~(1UL << (pos)))
	#define BIT_TOGGLE(byte , pos )		((byte) ^=  (1UL << (pos)))
	#define BIT_CHECK( byte , pos )		((byte) &   (1UL << (pos)))
/*
**************************************************************************
*                              FUNCTION PROTOTYPES
**************************************************************************
*/

/*
**************************************************************************
*                           GLOBAL FUNCTIONS
**************************************************************************
*/

void ds3231_PrintTime(	DS3231_TimeTypeDef 	*_timeSt,
						UART_HandleTypeDef	*_huart) {

	char DataChar[100];
	sprintf(DataChar,"%02d:%02d:%02d ",_timeSt->Hours, _timeSt->Minutes, _timeSt->Seconds);
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);
}
//************************************************************************

void ds3231_PrintDate(	DS3231_DateTypeDef		*_dateSt	,
						UART_HandleTypeDef	*_huart		) {

	char DataChar[100];
	sprintf(DataChar,"%02d/%02d/%04d ", _dateSt->Date, _dateSt->Month, 2000+ _dateSt->Year);
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);
}
//************************************************************************

void ds3231_PrintWeekAllChar(	DS3231_DateTypeDef 	*_dateSt	,
								UART_HandleTypeDef 	*_huart		) {

	char DataChar[100] ;
	switch(_dateSt->WeekDay) {
		case  0: sprintf( DataChar , "Sunday "		) ;		break ;
		case  1: sprintf( DataChar , "Monday "		) ;		break ;
		case  2: sprintf( DataChar , "Tuesday "		) ;		break ;
		case  3: sprintf( DataChar , "Wednesday "	) ;		break ;
		case  4: sprintf( DataChar , "Thursday "	) ;		break ;
		case  5: sprintf( DataChar , "Friday "		) ;		break ;
		case  6: sprintf( DataChar , "Saturday "	) ;		break ;
		case  7: sprintf( DataChar , "Sunday "		) ;		break ;
		default: sprintf( DataChar , "Out of day "	) ;		break ;
		} // end switch Date.ST
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);
}
//************************************************************************

void ds3231_PrintWeek3char(	DS3231_DateTypeDef 	*_dateSt	,
							UART_HandleTypeDef	*_huart		) {

	char DataChar[5] ;
	uint8_t day_u8 = ( _dateSt->WeekDay + 3 ) %6 ;
	switch(day_u8) {
		case  0: sprintf( DataChar , "SUN " ) ;	break ;
		case  1: sprintf( DataChar , "MON " ) ;	break ;
		case  2: sprintf( DataChar , "TUE " ) ;	break ;
		case  3: sprintf( DataChar , "WED " ) ;	break ;
		case  4: sprintf( DataChar , "THU " ) ;	break ;
		case  5: sprintf( DataChar , "FRI " ) ;	break ;
		case  6: sprintf( DataChar , "SAT " ) ;	break ;
		case  7: sprintf( DataChar , "SUN" ) ; 	break ;
		default: sprintf( DataChar , "ERR" ) ;	break ;
		} // end switch Date.ST
	HAL_UART_Transmit( _huart , (uint8_t *)DataChar , strlen(DataChar) , 100) ;
}
//************************************************************************

void ds3231_GetTime(	uint8_t 			_ds3231_i2c_adr	,
						DS3231_TimeTypeDef		*_timeSt		) {

	uint8_t ds3231_Seconds ;
	uint8_t ds3231_Minutes ;
	uint8_t ds3231_Hours   ;

	I2Cdev_readByte( _ds3231_i2c_adr, DS3231_SECONDS , &ds3231_Seconds, 100);
	I2Cdev_readByte( _ds3231_i2c_adr, DS3231_MINUTES , &ds3231_Minutes, 100);
	I2Cdev_readByte( _ds3231_i2c_adr, DS3231_HOUR	 , &ds3231_Hours,   100);

	_timeSt->Hours   = (ds3231_Hours   >> 4)*10 + (ds3231_Hours   &0x0F);
	_timeSt->Minutes = (ds3231_Minutes >> 4)*10 + (ds3231_Minutes &0x0F);
	_timeSt->Seconds = (ds3231_Seconds >> 4)*10 + (ds3231_Seconds &0x0F);
}
//************************************************************************

void ds3231_GetDate(	uint8_t 			_ds3231_i2c_adr	,
						DS3231_DateTypeDef		*_dateSt		) {

	uint8_t ds3231_WeekDay ;
	uint8_t ds3231_Date    ;
	uint8_t ds3231_Mouth   ;
	uint8_t ds3231_Year    ;

	I2Cdev_readByte( _ds3231_i2c_adr , DS3231_WEEKDAY, &ds3231_WeekDay, 100 ) ;
	I2Cdev_readByte( _ds3231_i2c_adr , DS3231_DATE	 , &ds3231_Date,    100 ) ;
	I2Cdev_readByte( _ds3231_i2c_adr , DS3231_MONTH	 , &ds3231_Mouth,   100 ) ;
	I2Cdev_readByte( _ds3231_i2c_adr , DS3231_YEAR	 , &ds3231_Year,    100 ) ;

	_dateSt->WeekDay = (ds3231_WeekDay >> 4) * 10 + (ds3231_WeekDay &0x0F ) ;
	_dateSt->Date    = (ds3231_Date    >> 4) * 10 + (ds3231_Date    &0x0F ) ;
	_dateSt->Month   = (ds3231_Mouth   >> 4) * 10 + (ds3231_Mouth   &0x0F ) ;
	_dateSt->Year    = (ds3231_Year    >> 4) * 10 + (ds3231_Year    &0x0F ) ;
}
//************************************************************************

void ds3231_SetTime(	uint8_t 			_ds3231_i2c_adr	,
						DS3231_TimeTypeDef		*_timeSt		) {

	DS3231_TimeTypeDef    TimeStr    ;
	TimeStr.Hours	= 16 * ( _timeSt->Hours   / 10 ) + ( _timeSt->Hours   % 10 ) ;
	TimeStr.Minutes	= 16 * ( _timeSt->Minutes / 10 ) + ( _timeSt->Minutes % 10 ) ;
	TimeStr.Seconds	= 16 * ( _timeSt->Seconds / 10 ) + ( _timeSt->Seconds % 10 ) ;

	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_SECONDS, TimeStr.Seconds ) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_MINUTES, TimeStr.Minutes ) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_HOUR	 , TimeStr.Hours   ) ;
}
//************************************************************************

void ds3231_SetDate(	uint8_t 			_ds3231_i2c_adr	,
						DS3231_DateTypeDef		*_dateSt		) {

	DS3231_DateTypeDef 	DateStr   ;
	DateStr.WeekDay = _dateSt->WeekDay ;
	DateStr.Year	= 16 * (_dateSt->Year  / 10 ) + (_dateSt->Year  % 10 ) ;
	DateStr.Month	= 16 * (_dateSt->Month / 10 ) + (_dateSt->Month % 10 ) ;
	DateStr.Date	= 16 * (_dateSt->Date  / 10 ) + (_dateSt->Date  % 10 ) ;

	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_WEEKDAY, DateStr.WeekDay ) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_DATE	 , DateStr.Date    ) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_MONTH  , DateStr.Month   ) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_YEAR   , DateStr.Year    );
}
//************************************************************************

void ds3231_Alarm1_SetEverySeconds(	uint8_t 	_ds3231_i2c_adr ) {
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_SECONDS		, 1UL<<7 ) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_MINUTES		, 1UL<<7 ) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_HOUR	 		, 1UL<<7 ) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_DAY_AND_DATE	, 1UL<<7 ) ;

	uint8_t alarm_status ;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100 ) ;
	alarm_status = alarm_status | ( 1UL<<DS3231_CONTROL_INTCN ) | ( 1UL<DS3231_CONTROL_A1IE ) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL, alarm_status ) ;
}
//************************************************************************

void ds3231_Alarm2_SetEveryMinutes( uint8_t 	_ds3231_i2c_adr) {
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_MINUTES		, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_HOUR	 		, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_DAY_AND_DATE	, 1UL<<7 );

	uint8_t alarm_status;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100   );
	alarm_status = alarm_status | (1UL<<DS3231_CONTROL_INTCN) | (1UL<DS3231_CONTROL_A2IE);
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL, alarm_status );
}
//************************************************************************

void ds3231_Alarm1_SetSeconds(	uint8_t 	_ds3231_i2c_adr	,
								uint8_t 	_second			) {

	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_SECONDS		, _second );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_MINUTES		, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_HOUR			, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_DAY_AND_DATE	, 1UL<<7 );

	uint8_t alarm_status;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100   );
	alarm_status = alarm_status | (1UL<<DS3231_CONTROL_INTCN) | (1UL<<DS3231_CONTROL_A1IE);
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL,  alarm_status );
}
//************************************************************************

void ds3231_Alarm2_SetMinuses(	uint8_t 	_ds3231_i2c_adr		,
								uint8_t 	_minutes			) {

	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_MINUTES		, _minutes );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_HOUR			, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_DAY_AND_DATE	, 1UL<<7 );

	uint8_t alarm_status;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100   );
	alarm_status = alarm_status | (1UL<<DS3231_CONTROL_INTCN) | (1UL<<DS3231_CONTROL_A2IE);
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL,  alarm_status );
}
//************************************************************************

void ds3231_Alarm1_SetHoursAndMinuses(	uint8_t 	_ds3231_i2c_adr	,
										uint8_t 	_hours			,
										uint8_t	 	_minutes		) {

	I2Cdev_writeByte( _ds3231_i2c_adr , DS3231_ALARM_1_SECONDS		, 0x00		) ;
	I2Cdev_writeByte( _ds3231_i2c_adr , DS3231_ALARM_1_MINUTES		, _minutes	) ;
	I2Cdev_writeByte( _ds3231_i2c_adr , DS3231_ALARM_1_HOUR			, _hours	) ;
	I2Cdev_writeByte( _ds3231_i2c_adr , DS3231_ALARM_1_DAY_AND_DATE	, 1UL<<7	) ;

	uint8_t alarm_status ;
	I2Cdev_readByte ( _ds3231_i2c_adr , DS3231_CONTROL, &alarm_status, 100   );
	alarm_status = alarm_status | (1UL<<DS3231_CONTROL_INTCN) | ( 1UL<<DS3231_CONTROL_A1IE ) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL,  alarm_status ) ;
}
//************************************************************************

void ds3231_Alarm2_SetHoursAndMinuses(	uint8_t 	_ds3231_i2c_adr	,
										uint8_t 	_hours			,
										uint8_t 	_minutes		) {

	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_MINUTES		, _minutes	) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_HOUR			, _hours	) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_DAY_AND_DATE	, 1UL<<7	) ;

	uint8_t alarm_status ;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100 ) ;
	alarm_status = alarm_status | (1UL<<DS3231_CONTROL_INTCN) | ( 1UL<<DS3231_CONTROL_A2IE ) ;
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL,  alarm_status ) ;
}
//************************************************************************


void ds3231_Alarm1_ClearStatusBit(uint8_t _ds3231_i2c_adr) {

	uint8_t status_bit;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL_STATUS, &status_bit, 100   );
	status_bit = status_bit & (0UL<<DS3231_CNTRL_STATUS_A1F);
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL_STATUS, status_bit );
}

void ds3231_Alarm2_ClearStatusBit(uint8_t _ds3231_i2c_adr) {

	uint8_t status_bit;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL_STATUS, &status_bit, 100   );
	status_bit = status_bit & (0UL<<DS3231_CNTRL_STATUS_A2F);
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL_STATUS, status_bit );
}
//************************************************************************

void ds3231_Alarm1_Stop(uint8_t _ds3231_i2c_adr) {

	uint8_t alarm_status ;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100   );
	// ??? I2Cdev_writeByte( _ds3231_i2c_adr, 0x0E,  alarm_status&&(0UL) );
	// ??? I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL,  0b00011111 );
}
//************************************************************************

void Set_Date_and_Time_by_str(	DS3231_DateTypeDef 	*_dateSt	,
								DS3231_TimeTypeDef 	*_timeSt	) {

	ds3231_SetDate(ADR_I2C_DS3231, _dateSt);
	ds3231_SetTime(ADR_I2C_DS3231, _timeSt);
}
//************************************************************************

void Set_Date_and_Time_to_DS3231(	uint16_t 	_year_u16	,
									uint8_t 	_month_u8	,
									uint8_t 	_day_u8		,
									uint8_t 	_weekday_u8	,
									uint8_t 	_hours_u8	,
									uint8_t 	_minutes_u8	,
									uint8_t 	_seconds_u8	) {

	DS3231_DateTypeDef DateStr   ;
	uint8_t _year_u8 = (uint8_t)(_year_u16 - 2000) ;
	DateStr.Year	= _year_u8		;
	DateStr.Month	= _month_u8		;
	DateStr.Date	= _day_u8		;
	DateStr.WeekDay	= _weekday_u8	;
	ds3231_SetDate( ADR_I2C_DS3231, &DateStr ) ;

	DS3231_TimeTypeDef    TimeStr    ;
	TimeStr.Hours   = _hours_u8   ;
	TimeStr.Minutes = _minutes_u8 ;
	TimeStr.Seconds = _seconds_u8 ;
	ds3231_SetTime(ADR_I2C_DS3231, &TimeStr);
}

//************************************************************************

uint8_t ds3231_Get_Alarm1_Status(uint8_t _ds3231_i2c_adr) {
	uint8_t alarm1_status_u8 = 0;
	uint8_t control_status_u8 = 0;
	I2Cdev_readByte( _ds3231_i2c_adr, DS3231_CONTROL_STATUS , &control_status_u8, 100);
	alarm1_status_u8 = 	BIT_CHECK(control_status_u8, DS3231_CONTROL_A1IE);
	return alarm1_status_u8;
}
//************************************************************************

uint8_t ds3231_Get_Alarm2_Status(uint8_t _ds3231_i2c_adr) {
	uint8_t alarm_status_u8 = 0;
	uint8_t control_status_u8 = 0;
	I2Cdev_readByte( _ds3231_i2c_adr, DS3231_CONTROL_STATUS , &control_status_u8, 100);
	alarm_status_u8 = 	BIT_CHECK(control_status_u8, DS3231_CONTROL_A2IE);
	return alarm_status_u8;
}
//************************************************************************

void Ds3231_hard_alarm_flag_Set (void) {
	ds3231_alarm_u8 = 1 ;
}
//************************************************************************

void Ds3231_hard_alarm_flag_Reset (void) {
	ds3231_alarm_u8 = 0 ;
}
//************************************************************************

uint8_t Ds3231_hard_alarm_flag_Status (void) {
	return ds3231_alarm_u8 ;
}
/*
**************************************************************************
*                                   END
**************************************************************************
*/
