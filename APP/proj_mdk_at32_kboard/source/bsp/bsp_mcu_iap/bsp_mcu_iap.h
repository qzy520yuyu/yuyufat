#ifndef __BSP_MCU_IAP_H__
#define __BSP_MCU_IAP_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "at32f425_conf.h"
typedef void (*iapfun)(void);

typedef enum {
    RESULT_SUCCESS = 0x00,       // 成功
    RESULT_CRC8_ERROR = 0x01,    // CRC8 错误
    RESULT_LENGTH_ERROR = 0x02,  // 长度错误
    RESULT_CRC16_ERROR = 0x03,   // CRC16 错误
    RESULT_SEQ_ERROR = 0x04,     // 包序号错误
    RESULT_TIMEOUT = 0x05,       // 等待超时
    RESULT_OTHER_ERROR = 0x06    // 其他错误
} result_code_t;

void app_load(u32 appxaddr);

#ifdef __cplusplus
}
#endif

#endif  

