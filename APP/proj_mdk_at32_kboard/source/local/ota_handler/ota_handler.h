#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include <stdint.h>


#pragma pack(1)

typedef struct {
	uint8_t  upd_flag;// 升级标志
	uint32_t total_packets;// 包总数
	uint32_t file_size;// 文件大小
	uint32_t fw_size;// 固件大小
} fw_info_t;

#pragma pack()


typedef struct {
	uint32_t file_size;// 文件大小
	uint32_t fw_size;  // 固件大小
	uint32_t bytes_recv;  // 已接收数据大小
	uint32_t total_packets;   // 包总数
	uint32_t current_packet;  // 当前包
	uint32_t next_packet;     // 下一包
	uint8_t status;           // 升级状态
	uint16_t retry_count;     // 重传次数
	uint32_t chunk_size;// 固件分片大小
	uint16_t timeout;
} ota_info_t;



typedef enum
{
	UPDATE_PRE = 0x01,      // 升级前
	UPDATE_ING,             // 升级中
	UPDATE_CHECK,           // 升级校验
	UPDATE_DONE,           // 升级完成
	UPDATE_FAIL,           // 升级失败
} update_status_type;


void ota_start(uint8_t *data, uint32_t len);
void ota_update(uint8_t *data, uint32_t len);
void set_ota_timeout(uint16_t data);
void ota_upgrade();
void ota_upgrade_flag();

#endif // UPGRADE_CONTROL_H
