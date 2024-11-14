#ifndef _BSP_MCU_ADC_H
#define _BSP_MCU_ADC_H

#define ADC_BUF_LEN 	(8)

#ifdef __cplusplus
 extern "C" {
#endif
 
	void bsp_mcu_adc_init(void);
 void bsp_mcu_adc_deinit();
#ifdef __cplusplus
}
#endif

#endif
