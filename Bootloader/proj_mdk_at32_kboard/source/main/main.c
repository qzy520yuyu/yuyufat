#include "bsp_iap.h"  
#include "bsp_flash.h"
#include <stdint.h>
#include <string.h>

#define APP_ADDR             0x08001000
#define APP_backup           0x08008400
#define Data_ADDR 		    	 0x0800F800

fw_info_t fw_info;
uint8_t buffer[1024];

int main(void)
{
	memcpy(&fw_info, (uint32_t *)Data_ADDR, sizeof(fw_info_t));
	if(fw_info.upd_flag == 0)
	{
				for(int i = 0; i < fw_info.total_packets; i++)
				{
						memcpy(buffer,&(*(uint32_t *)(APP_backup+i*1024)),1024);//复制固件到APP运行区
						flash_write(i,buffer);
				}
		crm_reset(); // 重置CRM
		app_load(APP_ADDR);  
	}
	else
		app_load(APP_ADDR);  // 加载应用程序
	
  while(1);
}

