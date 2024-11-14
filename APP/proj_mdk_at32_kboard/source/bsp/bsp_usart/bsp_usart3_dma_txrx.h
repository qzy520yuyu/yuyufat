#ifndef _BSP_USART3_DMA_TXRX_H_
#define _BSP_USART3_DMA_TXRX_H_

#include <stdint.h>

typedef void (*pfunc_data_call)(uint8_t*, uint32_t);

#ifdef __cplusplus
 extern "C" {
#endif

	 void bsp_usart3_dma_txrx_init(void);
	 
	 void bsp_usart3_update_trans(uint8_t *data, uint32_t len);
	 
	 void bsp_usart3_regist_data_call(pfunc_data_call p);

 
#ifdef __cplusplus
 }
#endif

#endif
