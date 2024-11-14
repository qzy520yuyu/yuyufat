#include "firmware.h"
#include <string.h>
#include <stdio.h>
#include "protocol_tlvc.h"
#include "bsp_usart3_dma_txrx.h"

extern uint8_t fboard_ver[];

//UID
#define UID_ADDRESS 0x1FFFF7E8

void send_ver()
{
	uint8_t msg_buf[30];
	uint8_t data[4];
	memcpy(&data,&fboard_ver, 4);
	uint16_t msg_len=prototol_tlvc_pack('$', 'F',"V",1, (uint8_t *)&data, 4, msg_buf,30);
	bsp_usart3_update_trans(msg_buf,msg_len);
}


void send_fw(uint32_t size,uint32_t crc32)
{
	uint8_t msg_buf[30];
	uint8_t buf[8];
	memcpy(&buf[0], &size, 4);
	memcpy(&buf[4], &crc32, 4);
	uint16_t msg_len=prototol_tlvc_pack('$', 'F',"F",1, (uint8_t *)&buf,8, msg_buf,30);
	bsp_usart3_update_trans(msg_buf,msg_len);
}


void send_uid()
{
	uint8_t msg_buf[30];
	uint8_t buf[20];
	buf[0]=12;
	for(int i = 0 ; i < 12; i++)
	{
		buf[i+1]=*(uint8_t *)(UID_ADDRESS+i);
	}
	uint16_t msg_len=prototol_tlvc_pack('$', 'F',"D",1, (uint8_t *)&buf, 13, msg_buf,30);
	bsp_usart3_update_trans(msg_buf,msg_len);

}