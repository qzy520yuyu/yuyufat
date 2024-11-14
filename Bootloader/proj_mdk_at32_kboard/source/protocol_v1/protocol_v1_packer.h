#ifndef _PROTOCOL_V1_PACKER_H_
#define _PROTOCOL_V1_PACKER_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "protocol_v1_define.h"

#ifdef __cplusplus
 extern "C" {
#endif

	 void protocol_v1_generate_checksum(uint8_t *data, uint16_t len, uint8_t *checksum);
	 
	 void protocol_v1_generate_message(uint8_t cmd_set, uint8_t cmd_id,uint8_t *data, uint16_t *len);
	 
#ifdef __cplusplus
 }
#endif

#endif
