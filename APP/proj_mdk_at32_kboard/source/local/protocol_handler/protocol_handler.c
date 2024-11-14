#include "protocol_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include "protocol_tlvc.h"
#include "ota_handler.h"
#include "led_handler.h"
#include "bsp_pixel_led.h"
#include "firmware.h"


extern  uint16_t send_hz;

//TLVC
#define TLVC_BUFLEN  1024+128
static void callback_tlv_s(uint8_t *data, uint32_t len);
static void callback_tlv_p(uint8_t *data, uint32_t len);
static void callback_tlv_pack_r(uint8_t *data, uint32_t len);
static void callback_tlv_pack_l(uint8_t *data, uint32_t len);
static void callback_tlv_v(uint8_t *data, uint32_t len);
static void callback_tlv_f(uint8_t *data, uint32_t len);
static void callback_tlv_d(uint8_t *data, uint32_t len);

protocol_tlvc_t 	decoder;
uint8_t tlvc_buffer[TLVC_BUFLEN];



element_tlvc_t msg_elems[] =
{
	{"S", 1, callback_tlv_s},
	{"P", 1, callback_tlv_p},
	{"R", 1, callback_tlv_pack_r},
	{"L", 1, callback_tlv_pack_l},
	{"V", 1, callback_tlv_v}, 
	{"D", 1, callback_tlv_d},
	{"F", 1, callback_tlv_f},
};


static void callback_tlv_s(uint8_t *data, uint32_t len)
{
		ota_start(data,len);
}

static void callback_tlv_v(uint8_t *data, uint32_t len)//读取版本号
{
		send_ver();
}

static void callback_tlv_p(uint8_t *data, uint32_t len)
{
	
}
static void callback_tlv_f(uint8_t *data, uint32_t len)//设置发送频率
{
		memcpy(&send_hz,data,2);
}

static void callback_tlv_d(uint8_t *data, uint32_t len)//读取UID
{
		send_uid();
}

static void callback_tlv_pack_r(uint8_t *data, uint32_t len)
{
		ota_update(data,len);
}

static void callback_tlv_pack_l(uint8_t *data, uint32_t len)
{
		led_set(data,len);
}


void tlvc_regist()
{
		protocol_tlvc_regist(&decoder,'$','F',msg_elems,7,tlvc_buffer,TLVC_BUFLEN);
}


