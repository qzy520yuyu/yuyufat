#include "at32f425_conf.h"
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "bsp_gpio_key.h"
#include "bsp_mcu_adc.h"
#include "bsp_usart3_dma_txrx.h"
#include "bsp_pixel_led.h"
#include "at32f425_clock.h"
#include "type_kboard.h"
#include "data_transer_local.h"
#include "protocol_v1_decoder.h"

#pragma pack(1)

#ifndef PIXEL_LED_MAX
#define PIXEL_LED_MAX		(21)
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

extern uint8_t key_matrix_cur[36];
extern __IO uint16_t adc_sample[ADC_BUF_LEN];

kboard_t kboard_cur, kboard_pre;
float key_resistor_var = 0.0f;
float key_resistor_avg = 0.0f;
float bias_vol = 0.0f;

protocol_v1_decoder_t 	decoder;
packed_pixel_led_t 		packed_pixel;

//static void convert_keys_to_notes()
//{
//	if (key_matrix_cur[3][2])
//	{
//		kboard_cur.note[0] = 1;
//	}
//	else
//	{
//		kboard_cur.note[0] = 0;
//	}
//	if (key_matrix_cur[4][2])
//	{
//		kboard_cur.note[1] = 1;
//	}
//	else
//	{
//		kboard_cur.note[1] = 0;
//	}	
//	if (key_matrix_cur[5][2])
//	{
//		kboard_cur.note[2] = 1;
//	}
//	else
//	{
//		kboard_cur.note[2] = 0;
//	}		
//	if (key_matrix_cur[6][2])
//	{
//		kboard_cur.note[3] = 1;
//	}
//	else
//	{
//		kboard_cur.note[3] = 0;
//	}
//	if (key_matrix_cur[2][2])
//	{
//		kboard_cur.note[4] = 1;
//	}
//	else
//	{
//		kboard_cur.note[4] = 0;
//	}
//	if (key_matrix_cur[1][2])
//	{
//		kboard_cur.note[5] = 1;
//	}
//	else
//	{
//		kboard_cur.note[5] = 0;
//	}	
//	if (key_matrix_cur[0][2])
//	{
//		kboard_cur.note[6] = 1;
//	}
//	else
//	{
//		kboard_cur.note[6] = 0;
//	}		

//	/////////////////m keys
//	// 
//	if (key_matrix_cur[3][1])
//	{
//		kboard_cur.key_0[0] = 1;
//	}
//	else
//	{
//		kboard_cur.key_0[0] = 0;
//	}
//	if (key_matrix_cur[4][0])
//	{
//		kboard_cur.key_0[1] = 1;
//	}
//	else
//	{
//		kboard_cur.key_0[1] = 0;
//	}	
//	if (key_matrix_cur[4][1])
//	{
//		kboard_cur.key_0[2] = 1;
//	}
//	else
//	{
//		kboard_cur.key_0[2] = 0;
//	}		
//	if (key_matrix_cur[5][0])
//	{
//		kboard_cur.key_0[3] = 1;
//	}
//	else
//	{
//		kboard_cur.key_0[3] = 0;
//	}
//	if (key_matrix_cur[5][1])
//	{
//		kboard_cur.key_0[4] = 1;
//	}
//	else
//	{
//		kboard_cur.key_0[4] = 0;
//	}
//	if (key_matrix_cur[6][0])
//	{
//		kboard_cur.key_0[5] = 1;
//	}
//	else
//	{
//		kboard_cur.key_0[5] = 0;
//	}	
//	if (key_matrix_cur[6][1])
//	{
//		kboard_cur.key_0[6] = 1;
//	}
//	else
//	{
//		kboard_cur.key_0[6] = 0;
//	}			

//	/////////////////7 keys
//	// 
//	if (key_matrix_cur[3][0])
//	{
//		kboard_cur.key_1[0] = 1;
//	}
//	else
//	{
//		kboard_cur.key_1[0] = 0;
//	}
//	if (key_matrix_cur[2][1])
//	{
//		kboard_cur.key_1[1] = 1;
//	}
//	else
//	{
//		kboard_cur.key_1[1] = 0;
//	}	
//	if (key_matrix_cur[2][0])
//	{
//		kboard_cur.key_1[2] = 1;
//	}
//	else
//	{
//		kboard_cur.key_1[2] = 0;
//	}		
//	if (key_matrix_cur[1][1])
//	{
//		kboard_cur.key_1[3] = 1;
//	}
//	else
//	{
//		kboard_cur.key_1[3] = 0;
//	}
//	if (key_matrix_cur[1][0])
//	{
//		kboard_cur.key_1[4] = 1;
//	}
//	else
//	{
//		kboard_cur.key_1[4] = 0;
//	}
//	if (key_matrix_cur[0][1])
//	{
//		kboard_cur.key_1[5] = 1;
//	}
//	else
//	{
//		kboard_cur.key_1[5] = 0;
//	}	
//	if (key_matrix_cur[0][0])
//	{
//		kboard_cur.key_1[6] = 1;
//	}
//	else
//	{
//		kboard_cur.key_1[6] = 0;
//	}		
//	
//	// 
//	if (key_matrix_cur[0][3])
//	{
//		kboard_cur.arpeggio[0] = 1;
//	}
//	else
//	{
//		kboard_cur.arpeggio[0] = 0;
//	}
//	if (key_matrix_cur[1][3])
//	{
//		kboard_cur.arpeggio[1] = 1;
//	}
//	else
//	{
//		kboard_cur.arpeggio[1] = 0;
//	}
//	if (key_matrix_cur[2][3])
//	{
//		kboard_cur.arpeggio[2] = 1;
//	}
//	else
//	{
//		kboard_cur.arpeggio[2] = 0;
//	}	
//	if (key_matrix_cur[3][3])
//	{
//		kboard_cur.arpeggio[3] = 1;
//	}
//	else
//	{
//		kboard_cur.arpeggio[3] = 0;
//	}
//	if (key_matrix_cur[4][3])
//	{
//		kboard_cur.arpeggio[4] = 1;
//	}
//	else
//	{
//		kboard_cur.arpeggio[4] = 0;		
//	}
//	if (key_matrix_cur[5][3])
//	{
//		kboard_cur.arpeggio[5] = 1;		
//	}
//	else
//	{
//		kboard_cur.arpeggio[5] = 0;		
//	}	
//	// pad
//	if (key_matrix_cur[6][3])
//	{
//		kboard_cur.drumpad = 1;		
//	}
//	else
//	{
//		kboard_cur.drumpad = 0;	
//	}	
//}
// 
static void bias_adc_update()
{
	float sum = 0;
	for (uint32_t i = 7; i < ADC_BUF_LEN; i+=8)
	{
		sum += adc_sample[i];
	}
	key_resistor_avg = sum /10;

	sum = 0;
	for (uint32_t i = 7; i < ADC_BUF_LEN; i+=8)
	{
		float dr = adc_sample[i] - key_resistor_avg;
		sum += sqrt(dr * dr);
	}
	key_resistor_var = sum /10;	
	bias_vol = key_resistor_avg / 4096.0f * 3.3f;

	if (key_resistor_var < 10.0f)
	{	
		if (bias_vol < 0.15f)
		{
			kboard_cur.bias = 0;
		}	
		else if ((bias_vol > 0.15f) && (bias_vol < 0.5f))
		{
			kboard_cur.bias = 1;
		}
		else if ((bias_vol > 0.5f) && (bias_vol < 0.9f))
		{
			kboard_cur.bias = 2;
		}	
		else if ((bias_vol > 0.9f) && (bias_vol < 1.15f))
		{
			kboard_cur.bias = 3;
		}		
		else if ((bias_vol > 1.15f) && (bias_vol < 1.4f))
		{
			kboard_cur.bias = 4;
		}		
		else if ((bias_vol > 1.4f) && (bias_vol < 1.8f))
		{
			kboard_cur.bias = 5;
		}			
		else if ((bias_vol > 1.8f) && (bias_vol < 2.1f))
		{
			kboard_cur.bias = 6;
		}				
		else if ((bias_vol > 2.1f) && (bias_vol < 2.3f))
		{
			kboard_cur.bias = 7;
		}			
		else if ((bias_vol > 2.3f) && (bias_vol < 2.9f))
		{
			kboard_cur.bias = 8;
		}				
	}
}

static void usart_data_call(uint8_t *data, uint32_t len)
{
	if (protocol_v1_decoder_run(&decoder, data[0]))
	{
		memcpy(&packed_pixel, decoder.data, decoder.length);
		if ((decoder.cmd_set == 0X02) 
		 && (decoder.cmd_id == 0X29))
		{		
			uint32_t led_index = 0;
			for (uint32_t i = 0; i < 10; i++)
			{
				if (i >= 2 && i <= 8)
				{
					bsp_pixel_led_update(led_index, packed_pixel.r[i], packed_pixel.g[i], packed_pixel.b[i]);
					led_index ++;
					bsp_pixel_led_update(led_index, packed_pixel.r[i], packed_pixel.g[i], packed_pixel.b[i]);
					led_index ++;					

				}
				else
				{
					bsp_pixel_led_update(led_index, packed_pixel.r[i], packed_pixel.g[i], packed_pixel.b[i]);
					led_index ++;
				}				
			}				
			bsp_pixel_led_flush();	
		}
	}
}

static void led_start_show()
{	
	for (uint32_t light = 0; light < 30; light++)
	{
		for (uint32_t i = 0; i < PIXEL_NUM; i++)
		{
			bsp_pixel_led_update(i, light, light, light);
		}
		bsp_pixel_led_flush();	
		delay_ms(20);	
	}
	delay_ms(200);	
}

void interaction_kboard_init()
{
	memset(&kboard_cur, 0, sizeof(kboard_cur)); 
	memset(&kboard_pre, 0, sizeof(kboard_pre)); 
	led_start_show();	
	bsp_usart3_regist_data_call(usart_data_call);
	protocol_v1_decoder_init(&decoder);		
}

void interaction_kboard_update()
{
	bsp_key_gpio_scan();	
	//convert_keys_to_notes();
	bias_adc_update();
	transer_send_keyboard(&kboard_cur);
	
#if 0
	static uint16_t index = 0;
	for (uint32_t i = 0; i < 10; i++)
	{
		if (i == index)
		{
			bsp_pixel_led_update(i, 0, 0, 100);
		} 
		else
		{
			bsp_pixel_led_update(i, 0, 0, 0);
		}
	}
	bsp_pixel_led_flush();
	static uint32_t cnt = 0;
	if (cnt ++ % 10 == 0)
	{
		index ++;
		if (index >= 10)
		{
			index = 0;
		}
	}
#endif	
}


