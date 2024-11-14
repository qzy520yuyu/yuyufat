#ifndef _BOARD_H_
#define _BOARD_H_
 

#include "bsp_gpio_key.h"
#include "bsp_usart3_dma_txrx.h"
#include "bsp_mcu_adc.h"
#include "bsp_pixel_led.h"
#include "at32f425_clock.h"
#include "bsp_mcu_tim.h"
#include <stdint.h>
#include <string.h>
#include "bsp_mcu_iap.h"    
#include "bsp_mcu_flash.h"
#include "firmware.h"

#ifdef __cplusplus
extern "C" {
#endif




void board_init(void);
	
#ifdef __cplusplus
}
#endif

#endif
