#include "board.h"
#include "kboard.h"
#include "ota_handler.h"


uint8_t fboard_ver[4]={0,0,0,1};//版本号
extern fw_info_t fw_info;
int main(void)
{
	nvic_vector_table_set(NVIC_VECTTAB_FLASH,0x1000);
	board_init();
	interaction_kboard_init();
	
//	fw_info.upd_flag = 0;
//	fw_info.file_size=25000;
//	fw_info.fw_size=25000;
//	fw_info.total_packets=25;
//	fw_info.crc32=0x00010001;
//	uint8_t buf[17]; 
//	memcpy(buf, &fw_info, sizeof(buf));
//	flash_8b_write(Data_ADDR, buf, sizeof(buf)); 
//	crm_reset();
//	app_load(0x8000000);
  while(1)
  {
  	interaction_kboard_update();
  }
}


 