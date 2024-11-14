#ifndef _CRC_H_
#define _CRC_H_

#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif
	uint32_t crc32(uint32_t addr, uint32_t Length);
	uint16_t crc16(uint8_t *buf, uint16_t length);
	uint8_t crc8(uint8_t *data, uint8_t length);
 
#ifdef __cplusplus
 }
#endif

#endif
