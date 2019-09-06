#ifndef DS3231_SM_H_INCLUDED
#define DS3231_SM_H_INCLUDED

typedef struct
{
	UART_HandleTypeDef	*uart;
	GPIO_TypeDef 		*busy_port;
    uint16_t 			busy_pin;
} yx5200_struct;

void ds3231_init (yx5200_struct *yx5200_handler);
void ds3231_with_index (yx5200_struct *yx5200_handler, uint8_t song_index_u8);

#endif // DS3231_SM_H_INCLUDED
