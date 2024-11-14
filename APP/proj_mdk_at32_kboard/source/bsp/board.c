#include "board.h"


void board_init()
{
	//bsp_led_gpio_init();
	system_clock_config();
	delay_init();
	bsp_usart3_dma_txrx_init();
	bsp_mcu_adc_init();
	bsp_pixel_led_init();
	bsp_key_gpio_init();
	bsp_tim_init();
	
}



