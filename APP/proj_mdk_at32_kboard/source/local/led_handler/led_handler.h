#ifndef LED_HANDLER_H
#define LED_HANDLER_H

#include <stdint.h>

#pragma pack(1)

#ifndef PIXEL_LED_MAX
#define PIXEL_LED_MAX		(14)
#endif
typedef struct
{
	uint32_t  	time_us;
	uint8_t 	number;
	uint8_t 	r[PIXEL_LED_MAX];
	uint8_t 	g[PIXEL_LED_MAX];
	uint8_t 	b[PIXEL_LED_MAX];
}packed_pixel_led_t;
#pragma pack()

void led_start_show();
void led_set(uint8_t *data, uint32_t len);
extern packed_pixel_led_t 		packed_pixel;

#endif // LED_DEVICE_H
