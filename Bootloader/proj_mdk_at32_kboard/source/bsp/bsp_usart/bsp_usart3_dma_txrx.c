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
        usart_flag_clear(USART3, USART_RDBF_FLAG);  // ����жϱ�־λ
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
  gpio_default_para_init(&gpio_init_struct);  // ��ʼ��GPIO��Ĭ�ϲ���

  /* ����USART1��TX/RX���� */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;  // ��������ǿ��Ϊ��ǿ
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;  // �����������Ϊ�������
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;  // ��������ģʽΪ����
  gpio_init_struct.gpio_pins = GPIO_PINS_4 | GPIO_PINS_5;  // ����Ҫ���õ�����Ϊ4�ź�5������ (USART1��TX��RX����)
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;  // ��ʹ����������������
  gpio_init(GPIOC, &gpio_init_struct);  // ��ʼ��GPIOA������

  /* ����GPIO���ŵĸ��ù��� */
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE4, GPIO_MUX_1);  // ����9�����ŵĸ��ù���ΪUSART1 TX
  gpio_pin_mux_config(GPIOC, GPIO_PINS_SOURCE5, GPIO_MUX_1);  // ����10�����ŵĸ��ù���ΪUSART1 RX
	
}

static void perhi_init(void)
{
	usart_init(USART3, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);/* ��ʼ��USART1���� */
	usart_transmitter_enable(USART3, TRUE);    /* ʹ��USART1���� */
	usart_receiver_enable(USART3, TRUE); /* ʹ��USART1���� */
	usart_interrupt_enable(USART3, USART_RDBF_INT, TRUE);  // ����USART1�Ľ��ջ������ǿ��ж�
	usart_dma_transmitter_enable(USART3, TRUE); /* ʹ��USART1��DMA���� */
	usart_enable(USART3, TRUE);    /* ʹ��USART1���� */
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
  dma_reset(DMA1_CHANNEL3);  // ����DMA1ͨ��3
  dma_default_para_init(&dma_init_struct);  // ��ʼ��DMA��Ĭ�ϲ���
  dma_init_struct.buffer_size = len;  // ����DMA����Ļ�������СΪUSART1 TX��������С
  dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;  // ����DMA����Ϊ���ڴ洫�䵽����
  dma_init_struct.memory_base_addr = (uint32_t)data;  // �����ڴ����ַΪUSART1��TX��������ַ
  dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;  // �����ڴ����ݿ��Ϊ�ֽ�
  dma_init_struct.memory_inc_enable = TRUE;  // �����ڴ��ַ����ģʽ
  dma_init_struct.peripheral_base_addr = (uint32_t)&USART3->dt;  // �����������ַΪUSART3�����ݼĴ���
  dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;  // �����������ݿ��Ϊ�ֽ�
  dma_init_struct.peripheral_inc_enable = FALSE;  // ���������ַ����ģʽ
  dma_init_struct.priority = DMA_PRIORITY_MEDIUM;  // ����DMA���ȼ�Ϊ�е�
  dma_init_struct.loop_mode_enable = FALSE;  // ����ѭ��ģʽ
  dma_init(DMA1_CHANNEL3, &dma_init_struct);  // ��ʼ��DMA1ͨ��3
	dma_flexible_config(DMA1, FLEX_CHANNEL3, DMA_FLEXIBLE_UART3_TX);  // ����DMA1ͨ��3ΪUSART3 TX���DMAͨ��
	dma_channel_enable(DMA1_CHANNEL3, TRUE); 
	
  // �ȴ�������� 
  while (!dma_flag_get(DMA1_FDT3_FLAG));
  // ������ɺ������־
  dma_flag_clear(DMA1_FDT3_FLAG);
}


void bsp_usart3_regist_data_call(pfunc_data_call p)
{
	p_call = p;
}


