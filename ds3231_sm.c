
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


typedef enum {
	DS3231_CNTRL_STATUS_A1F	= 0x00	,
	DS3231_CNTRL_STATUS_A2F			,
	DS3231_CNTRL_STATUS_BSY			,
	DS3231_CNTRL_STATUS_EN32KHZ		,
	DS3231_CNTRL_STATUS_EMPTY_4		,
	DS3231_CNTRL_STATUS_EMPTY_5		,
	DS3231_CNTRL_STATUS_EMPTY_6		,
	DS3231_CNTRL_STATUS_OSF
}	ds3231_register_cntrl_status ;

/*
**************************************************************************
*								    DEFINES
**************************************************************************
*/

	//	#define SET_BIT(var, pos) ((var) |= (1UL << (pos)))
		#define CLR_BIT(var, pos) (var &= ~(1UL << (pos)))
		#define CHECK_BIT(var, pos) (((var) & (1UL << (pos))) != 0)


/*
**************************************************************************
*                              FUNCTION PROTOTYPES
**************************************************************************
*/

void ds3231_SetTime(uint8_t _ds3231_i2c_adr, RTC_TimeTypeDef * _timeSt);
void ds3231_SetDate(uint8_t _ds3231_i2c_adr, RTC_DateTypeDef * _dateSt);

/*
**************************************************************************
*                           GLOBAL FUNCTIONS
**************************************************************************
*/

void ds3231_PrintTime(RTC_TimeTypeDef * _timeSt, UART_HandleTypeDef *_huart) {
	char DataChar[100];
	sprintf(DataChar,"%02d:%02d:%02d ",_timeSt->Hours, _timeSt->Minutes, _timeSt->Seconds);
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);
}
//************************************************************************

void ds3231_PrintDate(RTC_DateTypeDef * _dateSt, UART_HandleTypeDef *_huart) {
	char DataChar[100];
	sprintf(DataChar,"%02d/%02d/%04d ", _dateSt->Date, _dateSt->Month, 2000+ _dateSt->Year);
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);
}
//************************************************************************

void ds3231_PrintWeek(RTC_DateTypeDef * _dateSt, UART_HandleTypeDef *_huart) {
	char DataChar[100];
	switch(_dateSt->WeekDay) 		{
		case  0: sprintf(DataChar,"Sunday "); 		break;
		case  1: sprintf(DataChar,"Monday ");		break;
		case  2: sprintf(DataChar,"Tuesday "); 		break;
		case  3: sprintf(DataChar,"Wednesday ");		break;
		case  4: sprintf(DataChar,"Thursday ");		break;
		case  5: sprintf(DataChar,"Friday ");		break;
		case  6: sprintf(DataChar,"Saturday ");		break;
		case  7: sprintf(DataChar,"Sunday "); 		break;
		default: sprintf(DataChar,"Out of day ");	break;
		} // end switch Date.ST
	HAL_UART_Transmit(_huart, (uint8_t *)DataChar, strlen(DataChar), 100);
}
//************************************************************************

void ds3231_GetTime(uint8_t _ds3231_i2c_adr, RTC_TimeTypeDef * _timeSt) {
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

void ds3231_GetDate(uint8_t _ds3231_i2c_adr, RTC_DateTypeDef * _dateSt) {
	uint8_t ds3231_WeekDay ;
	uint8_t ds3231_Date    ;
	uint8_t ds3231_Mouth   ;
	uint8_t ds3231_Year    ;

	I2Cdev_readByte( _ds3231_i2c_adr, DS3231_WEEKDAY, &ds3231_WeekDay, 100);
	I2Cdev_readByte( _ds3231_i2c_adr, DS3231_DATE	, &ds3231_Date,    100);
	I2Cdev_readByte( _ds3231_i2c_adr, DS3231_MONTH	, &ds3231_Mouth,   100);
	I2Cdev_readByte( _ds3231_i2c_adr, DS3231_YEAR	, &ds3231_Year,    100);

	_dateSt->WeekDay = (ds3231_WeekDay >> 4)*10 + (ds3231_WeekDay &0x0F);
	_dateSt->Date    = (ds3231_Date    >> 4)*10 + (ds3231_Date    &0x0F);
	_dateSt->Month   = (ds3231_Mouth   >> 4)*10 + (ds3231_Mouth   &0x0F);
	_dateSt->Year    = (ds3231_Year    >> 4)*10 + (ds3231_Year    &0x0F);
}
//************************************************************************

void ds3231_SetTime(uint8_t _ds3231_i2c_adr, RTC_TimeTypeDef * _timeSt) {
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_SECONDS, _timeSt->Seconds );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_MINUTES, _timeSt->Minutes );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_HOUR	 , _timeSt->Hours   );
}
//************************************************************************

void ds3231_SetDate(uint8_t _ds3231_i2c_adr, RTC_DateTypeDef * _dateSt) {
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_WEEKDAY, _dateSt->WeekDay );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_DATE	 , _dateSt->Date    );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_MONTH  , _dateSt->Month   );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_YEAR   , _dateSt->Year    );
}
//************************************************************************

void ds3231_Alarm1_SetEverySeconds(uint8_t _ds3231_i2c_adr) {
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_SECONDS		, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_MINUTES		, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_HOUR	 		, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_DAY_AND_DATE	, 1UL<<7 );

	uint8_t alarm_status;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100   );
	alarm_status = alarm_status | (1UL<<DS3231_CONTROL_INTCN) | (1UL<DS3231_CONTROL_A1IE);
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL, alarm_status );
}
//************************************************************************

void ds3231_Alarm2_SetEveryMinutes(uint8_t _ds3231_i2c_adr) {
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_MINUTES		, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_HOUR	 		, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_DAY_AND_DATE	, 1UL<<7 );

	uint8_t alarm_status;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100   );
	alarm_status = alarm_status | (1UL<<DS3231_CONTROL_INTCN) | (1UL<DS3231_CONTROL_A2IE);
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL, alarm_status );
}
//************************************************************************

void ds3231_Alarm1_SetSeconds(uint8_t _ds3231_i2c_adr, uint8_t _second) {
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

void ds3231_Alarm2_SetMinuses(uint8_t _ds3231_i2c_adr, uint8_t _minutes) {
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_MINUTES		, _minutes );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_HOUR			, 1UL<<7 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_DAY_AND_DATE	, 1UL<<7 );

	uint8_t alarm_status;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100   );
	alarm_status = alarm_status | (1UL<<DS3231_CONTROL_INTCN) | (1UL<<DS3231_CONTROL_A2IE);
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL,  alarm_status );
}
//************************************************************************

void ds3231_Alarm1_SetHoursAndMinuses(uint8_t _ds3231_i2c_adr, uint8_t _hours, uint8_t _minutes) {
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_SECONDS		, 0x00 );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_MINUTES		, _minutes );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_HOUR			, _hours );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_1_DAY_AND_DATE	, 1UL<<7 );

	uint8_t alarm_status;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100   );
	alarm_status = alarm_status | (1UL<<DS3231_CONTROL_INTCN) | (1UL<<DS3231_CONTROL_A1IE);
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL,  alarm_status );
}
//************************************************************************

void ds3231_Alarm2_SetHoursAndMinuses (uint8_t _ds3231_i2c_adr, uint8_t _hours, uint8_t _minutes){
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_MINUTES		, _minutes );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_HOUR			, _hours );
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_ALARM_2_DAY_AND_DATE	, 1UL<<7 );

	uint8_t alarm_status;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100   );
	alarm_status = alarm_status | (1UL<<DS3231_CONTROL_INTCN) | (1UL<<DS3231_CONTROL_A2IE);
	I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL,  alarm_status );
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
	uint8_t alarm_status;
	I2Cdev_readByte ( _ds3231_i2c_adr, DS3231_CONTROL, &alarm_status, 100   );
	// ??? I2Cdev_writeByte( _ds3231_i2c_adr, 0x0E,  alarm_status&&(0UL) );
	// ??? I2Cdev_writeByte( _ds3231_i2c_adr, DS3231_CONTROL,  0b00011111 );
}
//************************************************************************

void Set_Date_and_Time_by_str(RTC_DateTypeDef * _dateSt, RTC_TimeTypeDef * _timeSt ) {
	_dateSt->Year  = 0x20;
	_dateSt->Month = 0x03;
	_dateSt->Date  = 0x23;
	ds3231_SetDate(ADR_I2C_DS3231, _dateSt);

	_timeSt->Hours		= 0x13 ;
	_timeSt->Minutes	= 0x55 ;
	_timeSt->Seconds	= 0x00 ;
	ds3231_SetTime(ADR_I2C_DS3231, _timeSt);
}
//************************************************************************

void Set_Date_and_Time_to_DS3231(uint16_t _year_u16, uint8_t _month_u8, uint8_t _date_u8, uint8_t _hours_u8, uint8_t _minutes_u8, uint8_t _seconds_u8) {

	RTC_DateTypeDef DateStr   ;
	uint8_t _year_u8 = (uint8_t)(_year_u16 - 2000);
	DateStr.Year  = ( _year_u8/10)*16 +  _year_u8%10 ;
	DateStr.Month = (_month_u8/10)*16 + _month_u8%10 ;
	DateStr.Date  = ( _date_u8/10)*16 +  _date_u8%10 ;
	ds3231_SetDate(ADR_I2C_DS3231, &DateStr);

	RTC_TimeTypeDef    TimeStr    ;
	TimeStr.Hours   = (  _hours_u8/10)*16 +   _hours_u8%10 ;
	TimeStr.Minutes = (_minutes_u8/10)*16 + _minutes_u8%10 ;
	TimeStr.Seconds = (_seconds_u8/10)*16 + _seconds_u8%10 ;
	ds3231_SetTime(ADR_I2C_DS3231, &TimeStr);
}

//************************************************************************

uint8_t ds3231_Get_Alarm1_Status(uint8_t _ds3231_i2c_adr) {
	uint8_t alarm1_status_u8 = 0;
	uint8_t control_status_u8 = 0;
	I2Cdev_readByte( _ds3231_i2c_adr, DS3231_CONTROL_STATUS , &control_status_u8, 100);
	alarm1_status_u8 = 	CHECK_BIT(control_status_u8, DS3231_CONTROL_A1IE);
	return alarm1_status_u8;
}
//************************************************************************

uint8_t ds3231_Get_Alarm2_Status(uint8_t _ds3231_i2c_adr) {
	uint8_t alarm_status_u8 = 0;
	uint8_t control_status_u8 = 0;
	I2Cdev_readByte( _ds3231_i2c_adr, DS3231_CONTROL_STATUS , &control_status_u8, 100);
	alarm_status_u8 = 	CHECK_BIT(control_status_u8, DS3231_CONTROL_A2IE);
	return alarm_status_u8;
}
//************************************************************************
//************************************************************************
