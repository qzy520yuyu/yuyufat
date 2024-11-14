#include "ota_handler.h"
#include "at32f425_conf.h"
#include <stdio.h>
#include "bsp_usart3_dma_txrx.h"
#include "bsp_mcu_iap.h"    
#include "bsp_mcu_flash.h"
#include "protocol_tlvc.h"
#include "crc.h"
#include "firmware.h"
ota_info_t fboard_ota={
.timeout=2000
};

fw_info_t fw_info;
uint8_t ota_buffer[1024];
static uint8_t packet_retry_count;
static void ota_task(uint8_t *data);

extern uint8_t status;
extern uint16_t tim_ms;//ms
extern protocol_tlvc_t 	decoder;


void data_ack(uint32_t data)
{
	uint8_t msg_buf[30];
	uint8_t buf[4];
	memcpy(buf, &data, 4);
	uint16_t msg_len=prototol_tlvc_pack('$', 'F',"R",1, (uint8_t *)&buf, 4, msg_buf,30);
	bsp_usart3_update_trans(msg_buf,msg_len);
}


void ota_upgrade_flag()//size crc32
{
	memcpy(&fw_info, (uint32_t *)Data_ADDR, sizeof(fw_info_t));
	if(fw_info.upd_flag == 0)
	{
        uint32_t crc = crc32(APP_ADDR, fw_info.file_size);
        send_fw(fw_info.file_size,crc);
        fw_info.upd_flag = 1;
        uint8_t buf[13];
        memcpy(buf,&fw_info,13);
        flash_8b_write(Data_ADDR,buf,13);
	}
}


void ota_start(uint8_t *data, uint32_t len)
{
	memcpy(&fboard_ota.file_size, &data[0], sizeof(uint32_t));// 拷贝升级包大小
	memcpy(&fboard_ota.chunk_size,&data[4], sizeof(uint32_t));//分包大小
	memcpy(&fboard_ota.total_packets, &data[8], sizeof(uint32_t));// 拷贝升级包总数
	status = 0x02;//OTA模式
	fboard_ota.fw_size = fboard_ota.file_size - 8; // 计算实际固件大小
	fboard_ota.status = UPDATE_PRE; // 状态-等待接收
	tim_ms = 0; // 重置计时器
	data_ack(fboard_ota.next_packet); // 发送包序号
}


void ota_update(uint8_t *data, uint32_t len)
{
    memcpy(&fboard_ota.current_packet, &data[0], sizeof(uint32_t));   // 获取包序号
    memcpy(ota_buffer, &data[4], len - 4);  // 拷贝数据到ota_buffer
    fboard_ota.bytes_recv += len - 4; // 已接收字节数
    fboard_ota.status = UPDATE_ING; // 状态-更新中
}


// OTA重发
static void ota_resend_logic()
{
    tim_ms = 0;
    tlvc_reset(&decoder);  // 复位解码器

    if (fboard_ota.retry_count < 3)
    {
        data_ack(fboard_ota.next_packet);   // 请求数据包重新发送
        fboard_ota.retry_count++;           // 增加重传次数
    }
    else
    {
        fboard_ota.status = UPDATE_FAIL;    // 超过重传次数，标记为失败
    }
}


void set_ota_timeout(uint16_t data)
{
    fboard_ota.timeout=data;
}

// OTA升级
void ota_upgrade()
{
    if (tim_ms > fboard_ota.timeout && fboard_ota.status == UPDATE_PRE)
    {
        ota_resend_logic();  // 处理重试逻辑
    }
    else if (fboard_ota.status == UPDATE_ING)
    {
        fboard_ota.retry_count = 0;  // 清空重传次数
    }

    ota_task(ota_buffer);  // OTA任务处理
}



static void ota_task(uint8_t *data)
{

    if (fboard_ota.status == UPDATE_ING) // 检查更新状态是否为正在更新
    {
        if (fboard_ota.file_size < fboard_ota.bytes_recv) // 检查接收的字节数是否超过文件大小
        {
            fboard_ota.status = UPDATE_FAIL; // 更新失败
        }

        else if (fboard_ota.current_packet != fboard_ota.next_packet) // 检查当前数据包和请求数据包是否相同
        {
            tim_ms = 0; // 重置定时器
            fboard_ota.status = UPDATE_PRE; // 设置为准备更新状态

            if (packet_retry_count < 3) // 检查重试次数是否小于3
            {
                data_ack(fboard_ota.next_packet); // 发送数据确认包
                packet_retry_count++; // 增加重试计数
            }
            else
            {
                fboard_ota.status = UPDATE_FAIL; // 更新失败
            }
        }
        else
        {
            flash_write(fboard_ota.current_packet, data); // 将当前数据包写入闪存
            if (fboard_ota.total_packets - 1 == fboard_ota.current_packet) // 检查当前包是否为最后一个包
            {
                fboard_ota.status = UPDATE_CHECK; // 设置为检查状态
            }
            else
            {
                tim_ms = 0; // 重置定时器
                packet_retry_count = 0; // 重置重试计数
                fboard_ota.status = UPDATE_PRE; // 设置为准备更新状态
                fboard_ota.next_packet = fboard_ota.current_packet + 1; // 更新下一个数据包
                data_ack(fboard_ota.next_packet); // 发送数据确认包
            }
        }
    }


    if (fboard_ota.status == UPDATE_CHECK) // 检查状态是否为检查更新
    {
        //CRC验证
        uint32_t crc = crc32(APP_backup, fboard_ota.fw_size); // 计算升级区的CRC值
        uint32_t fw_info_crc32 = *(uint32_t *)(APP_backup + fboard_ota.fw_size);

        //大小验证
        uint32_t size = *(uint32_t *)(APP_backup + fboard_ota.fw_size + 4);

        if (crc !=  fw_info_crc32  || size != fboard_ota.fw_size ) // 检查CRC值、大小
        {
            fboard_ota.status = UPDATE_FAIL; // 更新失败
        }
        else
        {
            fboard_ota.status = UPDATE_DONE; // 更新完成
        }
    }

    if (fboard_ota.status == UPDATE_DONE) // 更新完成
    {
		fw_info.upd_flag = 0; // 设置更新标志为0
		fw_info.file_size=fboard_ota.file_size;
		fw_info.fw_size=fboard_ota.fw_size;
		fw_info.total_packets=fboard_ota.total_packets;

		uint8_t buf[13];
		memcpy(buf, &fw_info, sizeof(buf));
		flash_8b_write(Data_ADDR, buf, sizeof(buf));

		crm_reset(); // 重置CRM
		app_load(0x8000000); // 加载Bootloader
    }

    if (fboard_ota.status == UPDATE_FAIL) // 更新失败
    {
        memset(&fboard_ota, 0, sizeof(fboard_ota)); 
        status = 0x01; // 设置状态为1
        fboard_ota.status = 0; // 将状态重置为0
    }
}



