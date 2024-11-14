/**
  **************************************************************************
  * @file     flash.h
  * @brief    flash header file
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

#ifndef __BSP_MCU_FLASH_H__
#define __BSP_MCU_FLASH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#define FLASH_SIZE    (*((uint32_t*)0x1FFFF7E0))  /* read from at32 flash capacity register(unit:kbyte) */
#define SRAM_SIZE     20                         /* sram size, unit:kbyte */


void flash_1kb_write(uint32_t write_addr, uint8_t *pbuffer);
void flash_8b_write(uint32_t write_addr, uint8_t *pbuffer, uint16_t num_write);
void flash_read(uint32_t read_addr, uint8_t *p_buffer, uint16_t num_read);
void flash_write(uint16_t packet, uint8_t *data);
#ifdef __cplusplus
}
#endif

#endif
