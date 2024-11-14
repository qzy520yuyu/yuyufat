#include "at32f425_conf.h"
#include "bsp_mcu_adc.h"
__IO uint16_t adc_sample[ADC_BUF_LEN]; // ADC采样缓冲区

void bsp_mcu_adc_init() 
{
   
  gpio_init_type gpio_initstructure; // 定义GPIO初始化结构体
	dma_init_type dma_init_struct; // 定义DMA初始化结构体
  adc_base_config_type adc_base_struct; // 定义ADC基础配置结构体
	
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE); // 启用DMA1时钟
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE); // 启用GPIOA时钟
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE); // 启用GPIOB时钟
  crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE); // 启用ADC1时钟
  crm_adc_clock_div_set(CRM_ADC_DIV_6); // 设置ADC时钟分频系数为6

  gpio_default_para_init(&gpio_initstructure); // 设置GPIO默认参数
  gpio_initstructure.gpio_mode = GPIO_MODE_ANALOG; // 将GPIO模式设置为模拟模式
  gpio_initstructure.gpio_pins = GPIO_PINS_1 | GPIO_PINS_2 | GPIO_PINS_3| GPIO_PINS_4 | GPIO_PINS_5 | GPIO_PINS_6| GPIO_PINS_7;
  gpio_init(GPIOA, &gpio_initstructure);

	gpio_initstructure.gpio_pins = GPIO_PINS_1 ;
  gpio_init(GPIOB, &gpio_initstructure); 
	

  dma_reset(DMA1_CHANNEL1); // 重置DMA1通道1
  dma_flexible_config(DMA1, FLEX_CHANNEL1, DMA_FLEXIBLE_ADC1); // 配置DMA灵活通道1为ADC1
  dma_default_para_init(&dma_init_struct); // 设置DMA默认参数
  dma_init_struct.buffer_size = ADC_BUF_LEN; // 设置DMA缓冲区大小为ADC_BUF_LEN
  dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY; // 设置DMA传输方向为从外设到内存
  dma_init_struct.memory_base_addr = (uint32_t)adc_sample; // 设置内存基地址为adc_sample数组的地址
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD; // 设置内存数据宽度为16位
  dma_init_struct.memory_inc_enable = TRUE; // 启用内存地址自增模式
  dma_init_struct.peripheral_base_addr = (uint32_t)&(ADC1->odt); // 设置外设基地址为ADC1的普通数据寄存器地址
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD; // 设置外设数据宽度为16位
  dma_init_struct.peripheral_inc_enable = FALSE; // 禁用外设地址自增模式
  dma_init_struct.priority = DMA_PRIORITY_HIGH; // 设置DMA优先级为高
  dma_init_struct.loop_mode_enable = TRUE; // 启用DMA循环模式
  dma_init(DMA1_CHANNEL1, &dma_init_struct); // 初始化DMA1通道1
  dma_channel_enable(DMA1_CHANNEL1, TRUE); // 启用DMA1通道1
	
  adc_base_default_para_init(&adc_base_struct); // 设置ADC默认参数
  adc_base_struct.sequence_mode = TRUE; // 启用ADC顺序模式
  adc_base_struct.repeat_mode = TRUE; // 启用ADC重复模式
  adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT; // 设置ADC数据对齐方式为右对齐
  adc_base_struct.ordinary_channel_length = 8; // 设置普通通道转换序列的长度为8
  adc_base_config(ADC1, &adc_base_struct); // 初始化ADC1
	
	// 设置ADC1的第1个普通通道为通道4，采样时间为239.5个时钟周期
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_1, 1, ADC_SAMPLETIME_239_5); 
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_2, 2, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_3, 3, ADC_SAMPLETIME_239_5); 
	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_4, 4, ADC_SAMPLETIME_239_5); 
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_5, 5, ADC_SAMPLETIME_239_5);
  adc_ordinary_channel_set(ADC1, ADC_CHANNEL_6, 6, ADC_SAMPLETIME_239_5); 
	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_7, 7, ADC_SAMPLETIME_239_5); 
	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_9, 8, ADC_SAMPLETIME_239_5); 
	
  adc_ordinary_conversion_trigger_set(ADC1, ADC12_ORDINARY_TRIG_SOFTWARE, TRUE); // 设置ADC普通通道的软件触发
  adc_dma_mode_enable(ADC1, TRUE); // 启用ADC的DMA模式
  adc_enable(ADC1, TRUE); // 启用ADC1
  adc_calibration_init(ADC1); // 初始化ADC校准
  while(adc_calibration_init_status_get(ADC1)); // 等待ADC校准初始化完成
  adc_calibration_start(ADC1); // 开始ADC校准
  while(adc_calibration_status_get(ADC1)); // 等待ADC校准完成
	adc_ordinary_software_trigger_enable(ADC1, TRUE);
	
}

void bsp_mcu_adc_deinit() 
{
	adc_ordinary_software_trigger_enable(ADC1, FALSE); // 停止ADC1软件转换
	adc_dma_mode_enable(ADC1, FALSE);   // 禁用ADC1 DMA
	adc_enable(ADC1, FALSE); // 禁用ADC1
	dma_channel_enable(DMA1_CHANNEL1, FALSE);// 关闭DMA1通道1
	dma_reset(DMA1_CHANNEL1);// 复位DMA1通道1
}