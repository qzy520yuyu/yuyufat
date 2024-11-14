#ifndef _BSP_GPIO_LED_H
#define _BSP_GPIO_LED_H

#include <stdint.h>
 
#ifdef __cplusplus
 extern "C" {
#endif
 
void bsp_led_gpio_init(void);
void bsp_led_turn_on(uint8_t id);
void bsp_led_turn_off(uint8_t id);
void bsp_led_toggle(uint8_t id);
	 
#ifdef __cplusplus
}
#endif

#endif
