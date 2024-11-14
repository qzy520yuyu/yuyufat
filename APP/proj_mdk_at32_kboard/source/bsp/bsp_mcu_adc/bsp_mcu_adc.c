#include "at32f425_conf.h"
#include "bsp_mcu_adc.h"
__IO uint16_t adc_sample[ADC_BUF_LEN]; 

void bsp_mcu_adc_init() 
{
   
	gpio_init_type gpio_initstructure;
	dma_init_type dma_init_struct;
	adc_base_config_type adc_base_struct;
	
	crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
	crm_adc_clock_div_set(CRM_ADC_DIV_6);

	gpio_default_para_init(&gpio_initstructure);
	gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG;
	gpio_initstructure.gpio_pins = GPIO_PINS_1 | GPIO_PINS_2 | GPIO_PINS_3| GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_6| GPIO_PINS_7;
	gpio_init(GPIOA, &gpio_initstructure);

	gpio_initstructure.gpio_pins = GPIO_PINS_1 ;
	gpio_init(GPIOB, &gpio_initstructure);

	dma_reset(DMA1_CHANNEL1);
	dma_flexible_config(DMA1, FLEX_CHANNEL1, DMA_FLEXIBLE_ADC1); 
	dma_default_para_init(&dma_init_struct);
	dma_init_struct.buffer_size = ADC_BUF_LEN;
	dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY; 
	dma_init_struct.memory_base_addr = (uint32_t)adc_sample;
	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD; 
	dma_init_struct.memory_inc_enable = TRUE;
	dma_init_struct.peripheral_base_addr = (uint32_t)&(ADC1->odt); 
	dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD; 
	dma_init_struct.peripheral_inc_enable = FALSE; 
	dma_init_struct.priority = DMA_PRIORITY_HIGH;
	dma_init_struct.loop_mode_enable = TRUE; 
	dma_init(DMA1_CHANNEL1, &dma_init_struct); 
	dma_channel_enable(DMA1_CHANNEL1, TRUE); 

	adc_base_default_para_init(&adc_base_struct); 
	adc_base_struct.sequence_mode = TRUE; 
	adc_base_struct.repeat_mode = TRUE;
	adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT; 
	adc_base_struct.ordinary_channel_length = 8; 
	adc_base_config(ADC1, &adc_base_struct); 

	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_1, 1, ADC_SAMPLETIME_239_5);
	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_2, 2, ADC_SAMPLETIME_239_5);
	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_3, 3, ADC_SAMPLETIME_239_5);
	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_4, 4, ADC_SAMPLETIME_239_5);
	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_5, 5, ADC_SAMPLETIME_239_5);
	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_6, 6, ADC_SAMPLETIME_239_5);
	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_7, 7, ADC_SAMPLETIME_239_5);
	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_9, 8, ADC_SAMPLETIME_239_5);

	adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE); 
	adc_dma_mode_enable(ADC1, TRUE); 
	adc_enable(ADC1, TRUE); 
	adc_calibration_init(ADC1); 
	while(adc_calibration_init_status_get(ADC1)); 
	adc_calibration_start(ADC1); 
	while(adc_calibration_status_get(ADC1)); 
	adc_ordinary_software_trigger_enable(ADC1, TRUE);
	
}

void bsp_mcu_adc_deinit() 
{
	adc_ordinary_software_trigger_enable(ADC1, FALSE); 
	adc_dma_mode_enable(ADC1, FALSE);   
	adc_enable(ADC1, FALSE); 
	dma_channel_enable(DMA1_CHANNEL1, FALSE);
	dma_reset(DMA1_CHANNEL1);
}