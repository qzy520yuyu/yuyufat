#include "bsp_mcu_iap.h"       
#include "bsp_mcu_flash.h"
#include "at32f425_conf.h"
#include "bsp_mcu_adc.h"
#include "string.h"


iapfun jump_to_app;
void app_load(uint32_t app_addr)
{
  if(((*(uint32_t*)app_addr) - 0x20000000) <= (SRAM_SIZE * 1024))
  {
		bsp_mcu_adc_deinit();
		crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, FALSE);
		crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, FALSE);
		crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, FALSE);
		crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, FALSE);
		crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, FALSE);
		crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, FALSE);
		crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, FALSE);
		crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, FALSE);
		crm_periph_clock_enable(CRM_USART3_PERIPH_CLOCK, FALSE);
		crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, FALSE);

    nvic_irq_disable(USART4_3_IRQn);
    __NVIC_ClearPendingIRQ(USART4_3_IRQn);
		nvic_irq_disable(TMR2_GLOBAL_IRQn);
    __NVIC_ClearPendingIRQ(TMR2_GLOBAL_IRQn);
		
    jump_to_app = (iapfun)*(uint32_t*)(app_addr + 4);
    __set_MSP(*(uint32_t*)app_addr);
    jump_to_app();
  }
}




