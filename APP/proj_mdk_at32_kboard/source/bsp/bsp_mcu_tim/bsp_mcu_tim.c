#include "at32f425_conf.h"
#include "bsp_mcu_tim.h"

uint16_t tim_ms = 0;
uint16_t hz_1000=0;


void bsp_tim_init()
{
		crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);
    tmr_base_init(TMR2, 1000 - 1, 72 - 1);
    tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);
    tmr_interrupt_enable(TMR2, TMR_OVF_INT, TRUE);
    tmr_flag_clear(TMR2, TMR_OVF_FLAG);
    tmr_counter_enable(TMR2, TRUE);
    nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
    nvic_irq_enable(TMR2_GLOBAL_IRQn, 1, 0);
}

void TMR2_GLOBAL_IRQHandler(void)
{
    if (tmr_flag_get(TMR2, TMR_OVF_FLAG))
    {
        tim_ms++;
				hz_1000++;

        tmr_flag_clear(TMR2, TMR_OVF_FLAG);
    }
}