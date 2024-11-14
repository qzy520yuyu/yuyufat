/**
  **************************************************************************
  * @file     at32f425_clock.c
  * @brief    system clock config program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* includes ------------------------------------------------------------------*/
#include "at32f425_clock.h"

/**
  * @brief  system clock config program
  * @note   the system clock is configured as follow:
  *         system clock (sclk)   = hext * pll_mult
  *         system clock source   = pll (hext)
  *         - hext                = HEXT_VALUE
  *         - sclk                = 96000000
  *         - ahbdiv              = 1
  *         - ahbclk              = 96000000
  *         - apb2div             = 1
  *         - apb2clk             = 96000000
  *         - apb1div             = 1
  *         - apb1clk             = 96000000
  *         - pll_mult            = 12
  *         - flash_wtcyc         = 2 cycle
  * @param  none
  * @retval none
  */
void system_clock_config(void)
{
  /* reset crm */
  crm_reset();

  /* config flash psr register */
  flash_psr_set(FLASH_WAIT_CYCLE_2);

  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

  /* wait till hext is ready */
  while(crm_hext_stable_wait() == ERROR)
  {
  }

  /* config pll clock resource */
  crm_pll_config(CRM_PLL_SOURCE_HEXT, CRM_PLL_MULT_12);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) != SET)
  {
  }

  /* config ahbclk */
  crm_ahb_div_set(CRM_AHB_DIV_1);

  /* config apb2clk, the maximum frequency of APB1/APB2 clock is 96 MHz  */
  crm_apb2_div_set(CRM_APB2_DIV_1);

  /* config apb1clk, the maximum frequency of APB1/APB2 clock is 96 MHz  */
  crm_apb1_div_set(CRM_APB1_DIV_1);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL)
  {
  }

  /* update system_core_clock global variable */
  system_core_clock_update();
}



#define STEP_DELAY_MS                    50
static __IO uint32_t fac_us;
static __IO uint32_t fac_ms;

void delay_init()
{
  systick_clock_source_config(SYSTICK_CLOCK_SOURCE_AHBCLK_NODIV);
  fac_us = system_core_clock / (1000000U);
  fac_ms = fac_us * (1000U);
}


void Delay_us(uint32_t nus)
{
  uint32_t temp;
  SysTick->LOAD = (uint32_t)(nus*fac_us);
  SysTick->VAL = 0x00;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;
  do
  {
    temp = SysTick->CTRL;
  }while((temp & 0x01) &&! (temp & (1<<16)));

  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
  SysTick->VAL = 0x00;
}

/**
  * @brief  Inserts a delay time.
  * @param  nms: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay_ms(uint16_t nms)
{
  uint32_t temp;
  while(nms)
  {
    if(nms > STEP_DELAY_MS)
    {
      SysTick->LOAD = (uint32_t)(STEP_DELAY_MS * fac_ms);
      nms -= STEP_DELAY_MS;
    }
    else
    {
      SysTick->LOAD = (uint32_t)(nms * fac_ms);
      nms = 0;
    }
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do
    {
      temp = SysTick->CTRL;
    }while( (temp & 0x01) && !(temp & (1<<16)) );

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
  }
}
