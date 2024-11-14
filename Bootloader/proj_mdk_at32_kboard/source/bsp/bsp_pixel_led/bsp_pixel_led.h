#ifndef _BSP_PIXEL_LED_H
#define _BSP_PIXEL_LED_H

#include <stdint.h>
 
//#define 	PIXEL_NUM	(10)
#define 	PIXEL_NUM	(1)
 
#ifdef __cplusplus
 extern "C" {
#endif
 
	 void bsp_pixel_led_init(void);
	 
	 void bsp_pixel_led_update(uint8_t i, uint8_t r, uint8_t g, uint8_t b);
 
	 void bsp_pixel_led_flush(void);
	 
#ifdef __cplusplus
}
#endif

#endif
