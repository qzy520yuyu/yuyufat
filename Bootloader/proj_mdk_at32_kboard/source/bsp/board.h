#ifndef _BOARD_H_
#define _BOARD_H_
 
#include "bsp_gpio_led.h"
#include "bsp_gpio_key.h"
#include "bsp_usart3_dma_txrx.h"
#include "bsp_mcu_adc.h"
#include "bsp_pixel_led.h"
#include "at32f425_clock.h"
 

#ifdef __cplusplus
extern "C" {
#endif

void board_init(void);
	
#ifdef __cplusplus
}
#endif

#endif
