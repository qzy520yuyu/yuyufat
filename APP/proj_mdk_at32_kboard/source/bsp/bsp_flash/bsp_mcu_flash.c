#include "bsp_mcu_flash.h"
#include "at32f425_conf.h"


void flash_1kb_write(uint32_t write_addr, uint8_t *pbuffer)
{
    uint16_t index, write_data;

    flash_unlock();
    flash_sector_erase(write_addr);

    for(index = 0; index < 1024; index += 2)
    {
        write_data = (pbuffer[index+1] << 8) + pbuffer[index];
        flash_halfword_program(write_addr, write_data);
        write_addr += sizeof(uint16_t);
    }
    flash_lock();
}

void flash_8b_write(uint32_t write_addr, uint8_t *pbuffer, uint16_t num_write)
{
    uint16_t index, write_data;

    flash_unlock();
    flash_sector_erase(write_addr);

    for(index = 0; index < num_write; index += 2)
    {
        write_data = (pbuffer[index+1] << 8) + pbuffer[index];
        flash_halfword_program(write_addr, write_data);
        write_addr += sizeof(uint16_t);
    }

    flash_lock();
}

void flash_read(uint32_t read_addr, uint8_t *p_buffer, uint16_t num_read)
{
    uint32_t i;
    for(i = 0; i < num_read; i++)
    {
        p_buffer[i] = *(uint8_t*)(read_addr);
        read_addr += 1;
    }
}


void flash_write(uint16_t packet, uint8_t *data)
{
	flash_1kb_write(0x08008400+packet*0x400, data);
}


