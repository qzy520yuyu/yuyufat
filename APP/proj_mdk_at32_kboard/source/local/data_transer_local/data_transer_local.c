#include "data_transer_local.h"
#include "protocol_tlvc.h"
#include "bsp_usart3_dma_txrx.h"
#include "type_kboard.h"
#include <string.h>
#include <stdio.h>

#pragma pack(1)

typedef struct
{
	uint32_t  	time_ms;
	uint8_t    packindex;
	uint8_t 	key_bits[5];
	uint8_t		key_adc[7];
}packed_keyboard_t;

#pragma pack()


packed_keyboard_t packed_keys;

void transer_send_keyboard(const kboard_t *data)
{
	uint8_t i;
	uint8_t	msg_buf[128];
	uint8_t	data_bits[5];
	uint8_t	data_adc[7];

	memset(&data_bits, 0, 5);
	memset(&data_adc, 0, 7);
	
	for (i = 0; i < 36; i++) {
		data_bits[i / 8] |= data->note[i] << (7-(i % 8));
	}
	data_bits[4]|=(data->bias & 0x0F);

	for(i = 0; i < 7; i++)
	{
		data_adc[i] = data->arpstate[i];
	}

	packed_keys.time_ms	= 0;//dev_time.millis();
	packed_keys.packindex = 43 ;
	memcpy(packed_keys.key_adc, data_adc, 7);
	memcpy(packed_keys.key_bits, data_bits, 5);
	uint16_t msg_len=prototol_tlvc_pack('$', 'F',"K",1, &packed_keys, sizeof(packed_keys), msg_buf,128);


	// send out
	bsp_usart3_update_trans(msg_buf, msg_len);
}



 
