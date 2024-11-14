#ifndef _PROTOCOL_TLVC_DECODER_H_
#define _PROTOCOL_TLVC_DECODER_H_

#include <stdint.h>
#include <string.h>

typedef void (*pfunc_tlv_callback)(uint8_t*, uint32_t);

typedef struct
{
	// TAG
	char 		*tag;
	uint32_t  	taglen;	
	pfunc_tlv_callback pfunc;
} element_tlvc_t;

typedef struct
{
	uint8_t  		*data;
	uint32_t  		bytes;
	uint32_t  		length;
} data_buffer_t;

typedef struct
{
	uint32_t 		state;
	uint8_t 		head0;
	uint8_t 		head1;
	uint8_t 		len_h;
	uint8_t 		len_l;
	uint8_t			index;

	uint8_t 		crc8;
	uint16_t 		crc16;

	uint16_t  		total_len;
	uint16_t  		value_len;
		
	data_buffer_t   buffer;	

	uint32_t 		elem_num;
	element_tlvc_t  *elems;	
} protocol_tlvc_t;

#ifdef __cplusplus
 extern "C" {
#endif


int32_t protocol_tlvc_regist(protocol_tlvc_t *tlvc, 
							const char head0,
							const char head1,
							element_tlvc_t elems[], 
							uint32_t elemsize, 
							uint8_t* buf, 
							uint32_t buflen);

int32_t prototol_tlvc_decode(protocol_tlvc_t *tlvc, uint8_t *data, uint32_t len);

int32_t prototol_tlvc_pack(const char head0,
							const char head1,
							const char *tag,
							uint32_t taglen,
							uint8_t *value,
							uint32_t valuelen,
							uint8_t *data,
							uint32_t datalen);

void tlvc_reset(protocol_tlvc_t *tlvc);

#ifdef __cplusplus
 }
#endif

#endif
