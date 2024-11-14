#include "at32f425_conf.h"
#include "at32f425_clock.h"
#include <math.h>
#include "bsp_usart3_dma_txrx.h"
#include "protocol_tlvc.h" 
#include "data_transer_local.h"
#include "protocol_handler.h"
#include "ota_handler.h"
#include "led_handler.h"
#include "key_handler.h"


#define STATUS_RUNNING  0x01  // 运行状态
#define STATUS_OTA      0x02  // OTA状态
uint8_t status=STATUS_RUNNING;//状态
uint16_t send_hz=100;

extern protocol_tlvc_t 	decoder;
extern uint16_t tim_ms;
extern uint16_t hz_1000;
extern kboard_t kboard_cur;


static void usart_data_call(uint8_t *data, uint32_t len)
{
	prototol_tlvc_decode(&decoder, data, len);
	if(status == STATUS_OTA)tim_ms = 0;
}

void interaction_kboard_init()
{
	memset(&kboard_cur, 0, sizeof(kboard_cur));
	led_start_show();	
	bsp_usart3_regist_data_call(usart_data_call);
	tlvc_regist();
	ota_upgrade_flag();
}
void set_send_hz(uint16_t data)
{
	send_hz=data;
}

void handle_running_state()
{
	if(hz_1000 > (1.0/(float)send_hz)*1000)
	{
		hz_1000=0;
	  bsp_key_gpio_scan();           
    convert_keys_to_notes();       
    bias_adc_update();             
    transer_send_keyboard(&kboard_cur);   
    arpstate_adc(&kboard_cur);      
	}      
}
  

void interaction_kboard_update()
{
    if (status == STATUS_RUNNING)
    {
        handle_running_state();  // 运行
    }
    else
    {
        ota_upgrade();  // OTA升级
    }
}