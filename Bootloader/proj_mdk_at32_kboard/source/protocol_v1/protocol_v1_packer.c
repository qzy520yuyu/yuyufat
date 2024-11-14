#include "protocol_v1_packer.h"
 
void protocol_v1_generate_checksum(uint8_t *data, uint16_t len, uint8_t *checksum)
{
	uint8_t ck_a = 0, ck_b = 0;
 
	for (uint16_t i = 2; i < 2 + 4 + len; i++)
	{
		ck_a += data[i];
		ck_b += ck_a;
	}
	
	checksum[0] = ck_a;
	checksum[1] = ck_b;
}

void protocol_v1_generate_message(uint8_t cmd_set, uint8_t cmd_id, uint8_t *data, uint16_t *len)
{
	// pure data length
	uint16_t msg_len = *len;
	
	data[0]		= PROPOCOL_V1_HEADER_A;
	data[1]		= PROPOCOL_V1_HEADER_B;
	data[2]		= cmd_set;
	data[3]		= cmd_id;	
	data[4]  	= (msg_len & 0XFF00) >> 8;
	data[5]  	= (msg_len & 0X00FF);
	
	// data 6 - len is copy before pack
	// ...
	
	// data end -1 ~ end is check sum
	uint8_t check_sum[2];
	protocol_v1_generate_checksum(data, msg_len, check_sum);
	
	data[6 + msg_len + 0] = check_sum[0];
	data[6 + msg_len + 1] = check_sum[1];
	
	// return total length of message
	*len = 6 + msg_len + 2;
}
 
