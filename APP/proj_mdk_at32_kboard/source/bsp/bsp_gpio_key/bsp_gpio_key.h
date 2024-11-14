#ifndef _BSP_GPIO_KEY_H
#define _BSP_GPIO_KEY_H


#include <stdint.h>
#ifdef __cplusplus
 extern "C" {
#endif
 
	void bsp_key_gpio_init(void);
	 
	void bsp_key_gpio_scan(void);
 
	uint8_t read_key(uint8_t id);
#ifdef __cplusplus
}
#endif

#endif
