#ifndef __BSP_IAP_H__              
#define __BSP_IAP_H__

#ifdef __cplusplus              
extern "C" {
#endif

#include <stdint.h>
#include "at32f425_conf.h"


#pragma pack(1)

typedef struct {
	uint8_t  upd_flag;// 升级标志
	uint32_t total_packets;// 包总数
	uint32_t file_size;// 文件大小
	uint32_t fw_size;// 固件大小
} fw_info_t;

#pragma pack()


typedef void (*iapfun)(void);
void app_load(u32 appxaddr);

#ifdef __cplusplus
}
#endif

#endif  

