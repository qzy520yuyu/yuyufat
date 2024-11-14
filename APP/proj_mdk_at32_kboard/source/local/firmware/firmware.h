#ifndef FIRMWARE_H
#define FIRMWARE_H

#include <stdint.h>


//Bootloader 0x08000000-0x080007FF  0x1000 4KB
//APP	0x08001000-0x08007BFF  0x7400 29KB
//APP1  0x08007C00-0x0800DFFF  0x7400 29KB
//DATA     0X0800F800-0x0800FFFF  0x800 2KB

#define APP_ADDR             0x08001000
#define APP_backup           0x08008400
#define Data_ADDR 		     0x0800F800


void send_uid();
void send_ver();
void send_fw(uint32_t size,uint32_t crc32);

#endif 
