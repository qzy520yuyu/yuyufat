#include "key_handler.h"
#include "at32f425_conf.h"
#include "at32f425_clock.h"
#include <math.h>
#include "bsp_gpio_key.h"
#include "bsp_mcu_adc.h"


uint8_t key_matrix_cur[36];
extern __IO uint16_t adc_sample[ADC_BUF_LEN];

kboard_t kboard_cur;
float key_resistor_var = 0.0f;
float key_resistor_avg = 0.0f;
float bias_vol = 0.0f;


void bsp_key_gpio_scan()
{
	uint8_t i;
	for (i = 0; i < 36; i++)
	{
		if(read_key(i) == 0)
		{
				key_matrix_cur[i] = 1;
		}
		else
		{
				key_matrix_cur[i] = 0;
		}
	}    
}

//key
void convert_keys_to_notes()
{

	if (key_matrix_cur[1])
	{
		kboard_cur.note[0] = 1;
	}
	else
	{
		kboard_cur.note[0] = 0;
	}

	if (key_matrix_cur[4])
	{
		kboard_cur.note[1] = 1;
	}
	else
	{
		kboard_cur.note[1] = 0;
	}

	if (key_matrix_cur[7])
	{
		kboard_cur.note[2] = 1;
	}
	else
	{
		kboard_cur.note[2] = 0;
	}

	if (key_matrix_cur[10])
	{
		kboard_cur.note[3] = 1;
	}
	else
	{
		kboard_cur.note[3] = 0;
	}

	if (key_matrix_cur[13])
	{
		kboard_cur.note[4] = 1;
	}
	else
	{
		kboard_cur.note[4] = 0;
	}

	if (key_matrix_cur[16])
	{
		kboard_cur.note[5] = 1;
	}
	else
	{
		kboard_cur.note[5] = 0;
	}

	if (key_matrix_cur[19])
	{
		kboard_cur.note[6] = 1;
	}
	else
	{
		kboard_cur.note[6] = 0;
	}

	if (key_matrix_cur[22])
	{
		kboard_cur.note[7] = 1;
	}
	else
	{
		kboard_cur.note[7] = 0;
	}

	if (key_matrix_cur[25])
	{
		kboard_cur.note[8] = 1;
	}
	else
	{
		kboard_cur.note[8] = 0;
	}

	if (key_matrix_cur[28])
	{
		kboard_cur.note[9] = 1;
	}
	else
	{
		kboard_cur.note[9] = 0;
	}

	if (key_matrix_cur[31])
	{
		kboard_cur.note[10] = 1;
	}
	else
	{
		kboard_cur.note[10] = 0;
	}

	if (key_matrix_cur[34])
	{
		kboard_cur.note[11] = 1;
	}
	else
	{
		kboard_cur.note[11] = 0;
	}


	if (key_matrix_cur[0])
	{
		kboard_cur.key_0[0] = 1;
	}
	else
	{
		kboard_cur.key_0[0] = 0;
	}

	if (key_matrix_cur[3])
	{
		kboard_cur.key_0[1] = 1;
	}
	else
	{
		kboard_cur.key_0[1] = 0;
	}

	if (key_matrix_cur[6])
	{
		kboard_cur.key_0[2] = 1;
	}
	else
	{
		kboard_cur.key_0[2] = 0;
	}

	if (key_matrix_cur[9])
	{
		kboard_cur.key_0[3] = 1;
	}
	else
	{
		kboard_cur.key_0[3] = 0;
	}

	if (key_matrix_cur[12])
	{
		kboard_cur.key_0[4] = 1;
	}
	else
	{
		kboard_cur.key_0[4] = 0;
	}

	if (key_matrix_cur[15])
	{
		kboard_cur.key_0[5] = 1;
	}
	else
	{
		kboard_cur.key_0[5] = 0;
	}

	if (key_matrix_cur[18])
	{
		kboard_cur.key_0[6] = 1;
	}
	else
	{
		kboard_cur.key_0[6] = 0;
	}

	if (key_matrix_cur[21])
	{
		kboard_cur.key_0[7] = 1;
	}
	else
	{
		kboard_cur.key_0[7] = 0;
	}

	if (key_matrix_cur[24])
	{
		kboard_cur.key_0[8] = 1;
	}
	else
	{
		kboard_cur.key_0[8] = 0;
	}

	if (key_matrix_cur[27])
	{
		kboard_cur.key_0[9] = 1;
	}
	else
	{
		kboard_cur.key_0[9] = 0;
	}

	if (key_matrix_cur[30])
	{
		kboard_cur.key_0[10] = 1;
	}
	else
	{
		kboard_cur.key_0[10] = 0;
	}

	if (key_matrix_cur[33])
	{
		kboard_cur.key_0[11] = 1;
	}
	else
	{
		kboard_cur.key_0[11] = 0;
	}


	if (key_matrix_cur[2])
	{
		kboard_cur.key_1[0] = 1;
	}
	else
	{
		kboard_cur.key_1[0] = 0;
	}

	if (key_matrix_cur[5])
	{
		kboard_cur.key_1[1] = 1;
	}
	else
	{
		kboard_cur.key_1[1] = 0;
	}

	if (key_matrix_cur[8])
	{
		kboard_cur.key_1[2] = 1;
	}
	else
	{
		kboard_cur.key_1[2] = 0;
	}

	if (key_matrix_cur[11])
	{
		kboard_cur.key_1[3] = 1;
	}
	else
	{
		kboard_cur.key_1[3] = 0;
	}

	if (key_matrix_cur[14])
	{
		kboard_cur.key_1[4] = 1;
	}
	else
	{
		kboard_cur.key_1[4] = 0;
	}

	if (key_matrix_cur[17])
	{
		kboard_cur.key_1[5] = 1;
	}
	else
	{
		kboard_cur.key_1[5] = 0;
	}

	if (key_matrix_cur[20])
	{
		kboard_cur.key_1[6] = 1;
	}
	else
	{
		kboard_cur.key_1[6] = 0;
	}

	if (key_matrix_cur[23])
	{
		kboard_cur.key_1[7] = 1;
	}
	else
	{
		kboard_cur.key_1[7] = 0;
	}

	if (key_matrix_cur[26])
	{
		kboard_cur.key_1[8] = 1;
	}
	else
	{
		kboard_cur.key_1[8] = 0;
	}

	if (key_matrix_cur[29])
	{
		kboard_cur.key_1[9] = 1;
	}
	else
	{
		kboard_cur.key_1[9] = 0;
	}

	if (key_matrix_cur[32])
	{
		kboard_cur.key_1[10] = 1;
	}
	else
	{
		kboard_cur.key_1[10] = 0;
	}

	if (key_matrix_cur[35])
	{
		kboard_cur.key_1[11] = 1;
	}
	else
	{
		kboard_cur.key_1[11] = 0;
	}
}



//0,0.25,0.56,0.80,1.12,1.33,1.65,1.88,2.23,2.47,2.76,2.93
void bias_adc_update()
{
	float sum = 0;
	for (uint32_t i = 7; i <= 79; i += 8)
	{
		sum += adc_sample[i];
	}
	key_resistor_avg = sum / 10;

	sum = 0;
	for (uint32_t i = 7; i <= 79; i += 8)
	{
		float dr = adc_sample[i] - key_resistor_avg;
		sum += sqrt(dr * dr);
	}
	key_resistor_var = sum / 10;	
	bias_vol = key_resistor_avg / 4096.0f * 3.3f;

	if (key_resistor_var < 10.0f)
	{	
		if (bias_vol < 0.125f)
		{
			kboard_cur.bias = 0;
		}
		else if ((bias_vol >= 0.125f) && (bias_vol < 0.405f))
		{
			kboard_cur.bias = 1;
		}
		else if ((bias_vol >= 0.405f) && (bias_vol < 0.68f))
		{
			kboard_cur.bias = 2;
		}    
		else if ((bias_vol >= 0.68f) && (bias_vol < 0.96f))
		{
			kboard_cur.bias = 3;
		}        
		else if ((bias_vol >= 0.96f) && (bias_vol < 1.225f))
		{
			kboard_cur.bias = 4;
		}        
		else if ((bias_vol >= 1.225f) && (bias_vol < 1.49f))
		{
			kboard_cur.bias = 5;
		}            
		else if ((bias_vol >= 1.49f) && (bias_vol < 1.765f))
		{
			kboard_cur.bias = 6;
		}                
		else if ((bias_vol >= 1.765f) && (bias_vol < 2.055f))
		{
			kboard_cur.bias = 7;
		}            
		else if ((bias_vol >= 2.055f) && (bias_vol < 2.35f))
		{
			kboard_cur.bias = 8;
		}            
		else if ((bias_vol >= 2.35f) && (bias_vol < 2.615f))
		{
			kboard_cur.bias = 9;
		}            
		else if ((bias_vol >= 2.615f) && (bias_vol < 2.845f))
		{
			kboard_cur.bias = 10;
		}
		else if ((bias_vol >= 2.845f) && (bias_vol < 3.100f))
		{
			kboard_cur.bias = 11;
		}
	}
}



void arpstate_adc(kboard_t *data)
{
	uint16_t sum=0;
	for (uint16_t j = 0; j < 7; j++)
	{
		for (uint16_t i = 0; i < 10; i++)
		{
			sum += adc_sample[i * 8 + j] >> 4;
		}
		data->arpstate[6-j]=sum;
		sum = 0;
	}
}
