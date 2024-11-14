#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include <stdint.h>


#pragma pack(1)

typedef struct {
	uint8_t  upd_flag;// ������־
	uint32_t total_packets;// ������
	uint32_t file_size;// �ļ���С
	uint32_t fw_size;// �̼���С
} fw_info_t;

#pragma pack()


typedef struct {
	uint32_t file_size;// �ļ���С
	uint32_t fw_size;  // �̼���С
	uint32_t bytes_recv;  // �ѽ������ݴ�С
	uint32_t total_packets;   // ������
	uint32_t current_packet;  // ��ǰ��
	uint32_t next_packet;     // ��һ��
	uint8_t status;           // ����״̬
	uint16_t retry_count;     // �ش�����
	uint32_t chunk_size;// �̼���Ƭ��С
	uint16_t timeout;
} ota_info_t;



typedef enum
{
	UPDATE_PRE = 0x01,      // ����ǰ
	UPDATE_ING,             // ������
	UPDATE_CHECK,           // ����У��
	UPDATE_DONE,           // �������
	UPDATE_FAIL,           // ����ʧ��
} update_status_type;


void ota_start(uint8_t *data, uint32_t len);
void ota_update(uint8_t *data, uint32_t len);
void set_ota_timeout(uint16_t data);
void ota_upgrade();
void ota_upgrade_flag();

#endif // UPGRADE_CONTROL_H
