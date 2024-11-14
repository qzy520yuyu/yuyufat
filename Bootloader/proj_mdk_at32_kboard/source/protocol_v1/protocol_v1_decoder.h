#ifndef _PROTOCOL_V1_DECODER_H_
#define _PROTOCOL_V1_DECODER_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
 
#define  V1_DECODER_MAX_DATA_L (1024)

typedef enum  
{
	E_LINK_DECODE_STA_RESET = 0,
	E_LINK_DECODE_STA_HEADER_A,
	E_LINK_DECODE_STA_HEADER_B,
	E_LINK_DECODE_STA_CMD_SET,
	E_LINK_DECODE_STA_CMD_ID,
	E_LINK_DECODE_STA_CMD_LENGTH_H,
	E_LINK_DECODE_STA_CMD_LENGTH_L,
	E_LINK_DECODE_STA_CMD_DATA,
	E_LINK_DECODE_STA_CMD_CHECK_SUM_A,
	E_LINK_DECODE_STA_CMD_CHECK_SUM_B,
}protocol_v1_decoder_sta_e;

typedef struct _protocol_v1_decoder
{
	uint8_t 	status;
	uint16_t 	index;
	uint8_t 	data[V1_DECODER_MAX_DATA_L];
	uint8_t 	cmd_set;
	uint8_t 	cmd_id;
	uint16_t 	length_h;
	uint16_t 	length_l;
	uint16_t  	length;
	uint8_t 	ck_sum_a;
	uint8_t 	ck_sum_b;	
}protocol_v1_decoder_t;
 
#ifdef __cplusplus
 extern "C" {
#endif

void protocol_v1_decoder_init(protocol_v1_decoder_t *decoder);
	 
void protocol_v1_decoder_reset(protocol_v1_decoder_t *decoder);
 
bool protocol_v1_decoder_run(protocol_v1_decoder_t *decoder, uint8_t ch);

#ifdef __cplusplus
 }
#endif

#endif
