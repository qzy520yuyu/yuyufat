#include "protocol_tlvc.h"
#include <string.h>
#include <stdlib.h>
#include "crc.h"
#define	TAG	 "tlvc"


// return : offset
// 
void tlvc_reset(protocol_tlvc_t *tlvc)
{
	tlvc->state = '0';
}

static uint32_t decode_switch(protocol_tlvc_t *tlvc, uint32_t offset, uint8_t *data, uint32_t len)
{
	uint32_t used_bytes = 1;
	switch (tlvc->state)
	{	
	case '0': // head0
		{
			if (data[offset] == tlvc->head0)
			{
				tlvc->state = '1';				
			}			
		} break;			
	case '1': // head1
		{
			if (data[offset] == tlvc->head1)
			{
				tlvc->state = 'L';
				tlvc->index= 0 ;
			}
			else
			{
				tlvc->state = '0';	
			}
		} break;			
	case 'L': // length
		{
			if(tlvc->index == 0)
			{
				tlvc->len_l= data[offset];
				tlvc->index = 1;
			}
			else if(tlvc->index == 1)
			{
				tlvc->len_h= data[offset];
				tlvc->state = '8';
			}
		} break;
	case '8': // head+length crc8
		{
			uint8_t crc8_data[4]={tlvc->head0, tlvc->head1, tlvc->len_l, tlvc->len_h};
			tlvc->crc8 = crc8(crc8_data,4);
			if (tlvc->crc8 == data[offset])
			{
				tlvc->state = 'V';	 
				tlvc->total_len = (tlvc->len_h << 8)+ tlvc->len_l;
				tlvc->value_len = tlvc->total_len - 7;
				tlvc->buffer.bytes = 0;
			}
			else
			{
				tlvc->state = '0';	
			}
		} break;		
	case 'V':  // value
		{
			uint32_t left_size = tlvc->value_len - tlvc->buffer.bytes;
			uint32_t data_size = len - offset;
			uint32_t copy_size = (left_size < data_size) ? (left_size) : (data_size);

			uint32_t buf_offset = tlvc->buffer.bytes;
			memcpy(tlvc->buffer.data + buf_offset, (data + offset), copy_size);

			tlvc->buffer.bytes += copy_size;
			if (tlvc->buffer.bytes == tlvc->value_len)
			{
				tlvc->state = 'C';		
				tlvc->index= 0 ;
			}
			else if (tlvc->buffer.bytes > tlvc->value_len)
			{
				tlvc->state = '0'; // size wrong
			}
			// !!! not 1 byte
			used_bytes = copy_size;

		} break;	
	case 'C': // CRC16 
		{						
			if(tlvc->index == 0)
			{
				tlvc->crc16=(uint16_t)(data[offset]);
				tlvc->index = 1;
			}
			else if(tlvc->index == 1)
			{
				tlvc->crc16|=(uint16_t)(data[offset]) << 8;
				uint16_t crc16_local = crc16(tlvc->buffer.data, tlvc->value_len);
				if (crc16_local != tlvc->crc16)
				{
					// crc16 error !
					//mlog_w(TAG, "crc16 failed:%d-%d", crc16_local, tlvc->crc16);
				}
				else
				{
					for (uint32_t i = 0; i < tlvc->elem_num; i++)
					{
						if (strncmp(tlvc->elems[i].tag,
									(const char*)tlvc->buffer.data,
									tlvc->elems[i].taglen) == 0)
						{
							uint8_t *val_ptr = tlvc->buffer.data + tlvc->elems[i].taglen;
							uint32_t val_len = tlvc->buffer.bytes - tlvc->elems[i].taglen;
							tlvc->elems[i].pfunc(val_ptr, val_len);
							break;
						}
					}
				}
				tlvc->state = '0';
			}
		} break;					
	default :break;
	}

	return (offset + used_bytes);
}

int32_t prototol_tlvc_decode(protocol_tlvc_t *tlvc, uint8_t *data, uint32_t len)
{
	uint32_t offset = 0;
	do
	{
		offset = decode_switch(tlvc, offset, data, len);
		//mlog_i(TAG, "offset:%d, state:%c", offset, tlvc->state);
	} while (offset < len);
    return 0;
}

int32_t protocol_tlvc_regist(protocol_tlvc_t *tlvc, 
							const char head0,
							const char head1,
							element_tlvc_t elems[], 
							uint32_t elemsize, 
							uint8_t* buf, 
							uint32_t buflen)
{    
	// state
	tlvc->state 	= '0';
	tlvc->head0 	= head0;
	tlvc->head1 	= head1;
	tlvc->len_h 	= 0;
	tlvc->len_l 	= 0;
	tlvc->crc8 		= 0;
	tlvc->crc16		= 0;
	tlvc->total_len	= 0;
	tlvc->value_len	= 0;

    // data buffer
    tlvc->buffer.data	= buf;
    tlvc->buffer.bytes 	= 0;
    tlvc->buffer.length = buflen;
    //mlog_i(TAG, "regist, buffer len = %d", tlvc->buffer.length);

    // elements
    tlvc->elem_num = elemsize;
    tlvc->elems    = (element_tlvc_t*)malloc(elemsize * sizeof(element_tlvc_t));
    if (tlvc->elems == NULL)
    {
        return -1;
    }


    for (uint32_t i = 0; i < tlvc->elem_num; i++)
    {
        tlvc->elems[i].tag     = elems[i].tag;
        tlvc->elems[i].taglen  = elems[i].taglen;
        tlvc->elems[i].pfunc   = elems[i].pfunc;

    }
    return 0;
}
	 
int32_t prototol_tlvc_pack(const char head0,
							const char head1,	
							const char *tag,
							uint32_t taglen,
							uint8_t *value, 
							uint32_t valuelen,
							uint8_t *data,
							uint32_t datalen)
{	
	uint32_t offset = 0;
	data[0] = head0;
	data[1] = head1;
 
	uint16_t pack_len = 5 + taglen + valuelen + 2;
	memcpy(data + 2, &pack_len, 2);

	uint8_t crc8_t = crc8(&data[0],4);
	data[4] = crc8_t;
 
 	offset = 5;
	memcpy(data + offset, tag, taglen);
	offset += taglen;

	memcpy(data + offset, value, valuelen);
	offset += valuelen;

	uint16_t crc16_val = crc16(data + 5, (taglen + valuelen));
	memcpy(data + offset, &crc16_val, 2);
	offset += 2;

	return offset;
}

