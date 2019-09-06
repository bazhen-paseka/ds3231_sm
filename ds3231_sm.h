#ifndef DS3231_SM_H_INCLUDED
#define DS3231_SM_H_INCLUDED

typedef struct
{
	uint8_t				i2c_adr;
} ds3231_struct;

void ds3231_GetTime(ds3231_struct *_ds3221_handler, RTC_TimeTypeDef * _timeSt, RTC_DateTypeDef * _dateSt, UART_HandleTypeDef *_huart);

#endif // DS3231_SM_H_INCLUDED
