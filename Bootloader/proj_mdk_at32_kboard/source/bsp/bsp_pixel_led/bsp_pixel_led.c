#include "at32f425_conf.h"
#include <string.h>
#include "bsp_pixel_led.h"



#define 	BIT_1 		(68)
#define 	BIT_0 		(30)

#define 	FRAME_LEN	(4 + 24 * PIXEL_NUM)

uint16_t 	TimerPeriod = 0;

uint16_t 	frames[FRAME_LEN];
 
uint8_t 	rbufer[PIXEL_NUM] = {0};
uint8_t 	gbufer[PIXEL_NUM] = {0};
uint8_t 	bbufer[PIXEL_NUM] = {0};



static void buffer_update(uint8_t* rbuf, uint8_t* gbuf, uint8_t* bbuf)
{			 
	uint32_t i = 0, j = 0;
	uint32_t id = 0;
	memset(frames, 0, sizeof(frames));

	for(i = 0; i < PIXEL_NUM; i++)
	{
		id = PIXEL_NUM - i - 1;			
		for(j = 0; j < 8; j++)
		{
			frames[3 + 24 * id + j]			= (gbuf[i] & (0x80 >> j)) ? BIT_1 : BIT_0;
			frames[3 + 24 * id + j + 8]		= (rbuf[i] & (0x80 >> j)) ? BIT_1 : BIT_0;
			frames[3 + 24 * id + j + 16]	= (bbuf[i] & (0x80 >> j)) ? BIT_1 : BIT_0;
		}
	}
	frames[0] = 0;
	frames[1] = 0;
	frames[2] = 0;
	frames[4 + 24 * PIXEL_NUM - 1] = 0; 
}


//uint16_t frames1[FRAME_LEN]={68,68,30,68,30};
static void dma_config()
{
		dma_init_type dma_init_struct;
		dma_reset(DMA1_CHANNEL2);		// 重置DMA1通道2
		dma_init_struct.buffer_size = FRAME_LEN;		// 设置DMA传输缓冲区大小为3
		dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;		// 设置DMA传输方向为内存到外设
		dma_init_struct.memory_base_addr = (uint32_t)frames;		// 设置DMA内存基址为frames数组
		dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;		// 设置DMA内存数据宽度为半字（16位）
		dma_init_struct.memory_inc_enable = TRUE;		// 启用DMA内存地址自增
		dma_init_struct.peripheral_base_addr = (uint32_t)&TMR1->c1dt;		// 设置DMA外设基址为TMR1的通道3数据寄存器地址
		dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;		// 设置DMA外设数据宽度为半字（16位）
		dma_init_struct.peripheral_inc_enable = FALSE;		// 禁用DMA外设地址自增
		dma_init_struct.priority = DMA_PRIORITY_HIGH;		// 设置DMA优先级为高等
		dma_init_struct.loop_mode_enable = FALSE;		// 启用DMA循环模式，持续不断地传输数据
		dma_init(DMA1_CHANNEL2, &dma_init_struct);		// 初始化DMA1通道5的配置
		dma_channel_enable(DMA1_CHANNEL2, TRUE);		// 启用DMA1通道1
		dma_flexible_config(DMA1, FLEX_CHANNEL2, DMA_FLEXIBLE_TMR1_OVERFLOW);		// 配置DMA1通道5为TMR1溢出DMA请求的灵活通道
		tmr_output_enable(TMR1, TRUE);		// 启用TMR1的输出
		tmr_counter_enable(TMR1, TRUE);		// 启用TMR1计数器
}

static void timer_config()
{
	uint16_t timer_period = 0;
	
	gpio_init_type gpio_init_struct;
	tmr_output_config_type tmr_output_struct;
	
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE8, GPIO_MUX_2);
	timer_period = (SystemCoreClock / 800000 ) - 1;
	tmr_base_init(TMR1, timer_period, 0);// 初始化TMR1的基本配置，设置定时周期为计算出来的timer_period，预分频器为0
	tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);		// 设置TMR1的计数方向为向上计数
	tmr_output_default_para_init(&tmr_output_struct);// 初始化TMR输出结构体的默认参数
	tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_B;// 设置输出模式为PWM模式B
	tmr_output_struct.oc_output_state = TRUE;	// 启用输出状态
	tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;	// 设置输出极性为低电平有效
	tmr_output_struct.oc_idle_state = TRUE;		// 设置空闲状态为高电平
	tmr_output_struct.occ_output_state = TRUE;		// 启用比较输出状态
	tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_LOW;		// 设置比较输出极性为低电平有效
	tmr_output_struct.occ_idle_state = FALSE;		
	tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_1, &tmr_output_struct);		// 配置TMR1的通道3，加载配置好的输出结构体参数
	tmr_dma_request_enable(TMR1, TMR_OVERFLOW_DMA_REQUEST, TRUE);	// 启用TMR1的溢出DMA请求
	
}

void bsp_pixel_led_init()
{
	dma_config();	
	timer_config();	
}
void bsp_pixel_led_update(uint8_t i, uint8_t r, uint8_t g, uint8_t b)
{
 	if (i < PIXEL_NUM)
	{
		rbufer[i] = r;
		gbufer[i] = g;
		bbufer[i] = b;
	}
}

void bsp_pixel_led_flush()
{
	buffer_update(rbufer, gbufer, bbufer);
	dma_config();		
}

