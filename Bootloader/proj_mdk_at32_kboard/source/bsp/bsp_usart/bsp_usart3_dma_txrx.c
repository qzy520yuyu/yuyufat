#include "at32f425_conf.h"
#include <stddef.h>
#include "bsp_usart3_dma_txrx.h"

pfunc_data_call	p_call = NULL;

void USART4_3_IRQHandler(void)
{
    if(usart_interrupt_flag_get(USART3, USART_RDBF_FLAG) != RESET)
    {
        uint8_t data = usart_data_receive(USART3);
        if (p_call)
        {
            p_call(&data, 1);
        }
        usart_flag_clear(USART3, USART_RDBF_FLAG);  // 清除中断标志位
    }
}

static void rcc_init(void)
{
    crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
    crm_periph_clock_enable(CRM_USART3_PERIPH_CLOCK, TRUE);
}

static void nvic_init(void)
{ 
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_0);
    nvic_irq_enable(USART4_3_IRQn, 0, 0);
}

static void gpio_init1()
{
	gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);  // 初始化GPIO的默认参数

  /* 配置USART1的TX/RX引脚 */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;  // 设置驱动强度为较强
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;  // 设置输出类型为推挽输出
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;  // 设置引脚模式为复用
  gpio_init_struct.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5;  // 设置要配置的引脚为4号和5号引脚 (USART1的TX和RX引脚)
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;  // 不使用上拉或下拉电阻
  gpio_init(GPIOC, &gpio_init_struct);  // 初始化GPIOA的引脚

  /* 配置GPIO引脚的复用功能 */
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE4, GPIO_MUX_1);  // 配置9号引脚的复用功能为USART1 TX
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE5, GPIO_MUX_1);  // 配置10号引脚的复用功能为USART1 RX
	
}

static void perhi_init(void)
{
	usart_init(USART3, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);/* 初始化USART1参数 */
	usart_transmitter_enable(USART3, TRUE);    /* 使能USART1发送 */
	usart_receiver_enable(USART3, TRUE); /* 使能USART1接收 */
	usart_interrupt_enable(USART3, USART_RDBF_INT, TRUE);  // 启用USART1的接收缓冲区非空中断
	usart_dma_transmitter_enable(USART3, TRUE); /* 使能USART1的DMA发送 */
	usart_enable(USART3, TRUE);    /* 使能USART1外设 */
}



void bsp_usart3_dma_txrx_init()
{
	rcc_init();
	nvic_init();
	gpio_init1();	
	perhi_init(); 		
}

void bsp_usart3_update_trans(uint8_t *data, uint32_t len)
{
  dma_init_type dma_init_struct;
  dma_reset(DMA1_CHANNEL3);  // 重置DMA1通道3
  dma_default_para_init(&dma_init_struct);  // 初始化DMA的默认参数
  dma_init_struct.buffer_size = len;  // 设置DMA传输的缓冲区大小为USART1 TX缓冲区大小
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;  // 设置DMA方向为从内存传输到外设
  dma_init_struct.memory_base_addr = (uint32_t)data;  // 设置内存基地址为USART1的TX缓冲区地址
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;  // 设置内存数据宽度为字节
  dma_init_struct.memory_inc_enable = TRUE;  // 启用内存地址递增模式
  dma_init_struct.peripheral_base_addr = (uint32_t)&USART3->dt;  // 设置外设基地址为USART3的数据寄存器
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;  // 设置外设数据宽度为字节
  dma_init_struct.peripheral_inc_enable = FALSE;  // 禁用外设地址递增模式
  dma_init_struct.priority = DMA_PRIORITY_MEDIUM;  // 设置DMA优先级为中等
  dma_init_struct.loop_mode_enable = FALSE;  // 禁用循环模式
  dma_init(DMA1_CHANNEL3, &dma_init_struct);  // 初始化DMA1通道3
	dma_flexible_config(DMA1, FLEX_CHANNEL3, DMA_FLEXIBLE_UART3_TX);  // 配置DMA1通道3为USART3 TX灵活DMA通道
	dma_channel_enable(DMA1_CHANNEL3, TRUE); 
	
  // 等待传输完成 
  while (!dma_flag_get(DMA1_FDT3_FLAG));
  // 传输完成后，清除标志
  dma_flag_clear(DMA1_FDT3_FLAG);
}


void bsp_usart3_regist_data_call(pfunc_data_call p)
{
	p_call = p;
}


