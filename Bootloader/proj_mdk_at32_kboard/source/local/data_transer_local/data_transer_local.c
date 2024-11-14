#include "data_transer_local.h"
#include "protocol_v1_packer.h"
#include "bsp_usart3_dma_txrx.h"
#include "type_kboard.h"

#pragma pack(1)

typedef struct
{
	uint32_t  	time_us;
	uint32_t 	key_bits[2];
}packed_keyboard_t;

#pragma pack()
//uint64_t kk=0x123456789456;
static uint8_t 	msg_buf[128];
uint32_t total_key_bits[2] = {0};

void transer_send_keyboard(const kboard_t *data)
{	
	uint8_t i;
	packed_keyboard_t packed_keys;		
	
	//total_key_bits = 0;
	memset(&total_key_bits, 0, sizeof(total_key_bits));
	uint32_t  data_bits = 0;
	for (i = 0; i < 12; i++)
	{	
		data_bits |= data->note[i] << (i + 1);		
	}
	total_key_bits[0] |= data_bits << 19;//31 30 29 28 27 26 25 24 23 22 21 20

	data_bits = 0;
	for (i = 0; i < 12; i++)
	{	
		data_bits |= data->key_0[i] << (i + 1);		
	}
	total_key_bits[0] |= data_bits << 7;//19 18 17 16 15 14 13 12 11 10 9 8 

	data_bits = 0;
	for (i = 0; i < 8; i++)
	{	
		data_bits |= data->key_1[4+i] << i;		
	}
	total_key_bits[0] |= data_bits;	//7 6 5 4 3 2 1 0
	data_bits = 0;
	for(i=0;i<4;i++)
	{
		data_bits |= data->key_1[i] << (i + 1);
	}
	total_key_bits[1] |= data_bits<<27;//31 30 29 28 

	data_bits = 0;
	for (i = 0; i < 6; i++)
	{	
		data_bits |= data->arpeggio[i] << (i + 1);		
	}
	total_key_bits[1] |= data_bits << 21;	//27 26 25 24 23 22

	total_key_bits[1] |= (data->drumpad << 20); //21
	total_key_bits[1] |= (data->bias<<19); 
	
	packed_keys.time_us	= 0;//dev_time.millis();
	packed_keys.key_bits[0] = total_key_bits[0];
	packed_keys.key_bits[1] = total_key_bits[1];
 	 
	uint16_t 	msg_len = sizeof(packed_keys);
	
	// copy data
	memcpy(msg_buf + 6, &packed_keys, msg_len);
	protocol_v1_generate_message(0X02, 0X2A, msg_buf, &msg_len);
	
	// send out
	//bsp_usart3_update_trans(msg_buf, msg_len);
}
 
