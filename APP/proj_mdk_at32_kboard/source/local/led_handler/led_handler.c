#include "led_handler.h"
#include <string.h>
#include "bsp_pixel_led.h"
#include "at32f425_conf.h"
#include "at32f425_clock.h"

packed_pixel_led_t 		packed_pixel;

void led_start_show()
{	
	for (uint32_t light = 0; light < 30; light++)
	{
		for (uint32_t i = 0; i < PIXEL_LED_MAX; i++)
		{
			bsp_pixel_led_update(i, light, light, light);
		}
		bsp_pixel_led_flush();	
		Delay_ms(20);	
	}
	Delay_ms(200);	
}



void led_set(uint8_t *data, uint32_t len)
{
		memcpy(&packed_pixel, &data[0], len);
		for (uint32_t i = 0; i < PIXEL_NUM; i++)
		{
				bsp_pixel_led_update(i, packed_pixel.r[i], packed_pixel.g[i], packed_pixel.b[i]);
		}
		bsp_pixel_led_flush(); 
}