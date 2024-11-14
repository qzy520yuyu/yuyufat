#include "at32f425_conf.h"
#include "bsp_gpio_key.h"
#include <stdint.h>


void bsp_key_gpio_init()
{
  gpio_init_type gpio_init_struct;
  
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE); 
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE); 
  crm_periph_clock_enable(CRM_GPIOF_PERIPH_CLOCK, TRUE); 
	

  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL; 
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pull = GPIO_PULL_UP;


  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_11 |
                               GPIO_PINS_12  | GPIO_PINS_15;
  gpio_init(GPIOA, &gpio_init_struct); 
  

  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_2| GPIO_PINS_3 |GPIO_PINS_4 |
															 GPIO_PINS_5| GPIO_PINS_6 | GPIO_PINS_7 |GPIO_PINS_8 | 
															 GPIO_PINS_9| GPIO_PINS_10| GPIO_PINS_11|GPIO_PINS_12|
															 GPIO_PINS_13| GPIO_PINS_14 |GPIO_PINS_15;
  gpio_init(GPIOB, &gpio_init_struct); 
  

  gpio_init_struct.gpio_pins = GPIO_PINS_0 | GPIO_PINS_1 | GPIO_PINS_2 | GPIO_PINS_3 |
                               GPIO_PINS_6 | GPIO_PINS_7 | GPIO_PINS_8 | GPIO_PINS_9 |
															 GPIO_PINS_10 | GPIO_PINS_11 | GPIO_PINS_12 | GPIO_PINS_13 ;
  gpio_init(GPIOC, &gpio_init_struct); 
  
	gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init(GPIOD, &gpio_init_struct); 

  gpio_init_struct.gpio_pins = GPIO_PINS_6 | GPIO_PINS_7;
  gpio_init(GPIOF, &gpio_init_struct); 
}


uint8_t read_key(uint8_t id)
{
    switch(id)
    {
        // KEY1到KEY36
        case 0:  return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_4) == SET);   // KEY1 - PB4
        case 1:  return (gpio_input_data_bit_read(GPIOA, GPIO_PINS_9) == SET);   // KEY2 - PA9
        case 2:  return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_0) == SET);   // KEY3 - PB0
        case 3:  return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_5) == SET);   // KEY4 - PB5
        case 4:  return (gpio_input_data_bit_read(GPIOA, GPIO_PINS_10) == SET);  // KEY5 - PA10
        case 5:  return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_2) == SET);   // KEY6 - PB2
        case 6:  return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_6) == SET);   // KEY7 - PB6
        case 7:  return (gpio_input_data_bit_read(GPIOA, GPIO_PINS_11) == SET);  // KEY8 - PA11
        case 8:  return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_10) == SET);  // KEY9 - PB10
        case 9:  return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_7) == SET);   // KEY10 - PB7
        case 10: return (gpio_input_data_bit_read(GPIOA, GPIO_PINS_12) == SET);  // KEY11 - PA12
        case 11: return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_11) == SET);  // KEY12 - PB11
        case 12: return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_8) == SET);   // KEY13 - PB8
        case 13: return (gpio_input_data_bit_read(GPIOF, GPIO_PINS_6) == SET);   // KEY14 - PF6
        case 14: return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_12) == SET);  // KEY15 - PB12
        case 15: return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_9) == SET);   // KEY16 - PB9
        case 16: return (gpio_input_data_bit_read(GPIOF, GPIO_PINS_7) == SET);   // KEY17 - PF7
        case 17: return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_13) == SET);  // KEY18 - PB13
        case 18: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_13) == SET);  // KEY19 - PC13
        case 19: return (gpio_input_data_bit_read(GPIOA, GPIO_PINS_15) == SET);  // KEY20 - PA15
        case 20: return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_14) == SET);  // KEY21 - PB14
        case 21: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_0) == SET);   // KEY22 - PC0
        case 22: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_10) == SET);  // KEY23 - PC10
        case 23: return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_15) == SET);  // KEY24 - PB15
        case 24: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_1) == SET);   // KEY25 - PC1
        case 25: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_11) == SET);  // KEY26 - PC11
        case 26: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_6) == SET);   // KEY27 - PC6
        case 27: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_2) == SET);   // KEY28 - PC2
        case 28: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_12) == SET);  // KEY29 - PC12
        case 29: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_7) == SET);   // KEY30 - PC7
        case 30: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_3) == SET);   // KEY31 - PC3
        case 31: return (gpio_input_data_bit_read(GPIOD, GPIO_PINS_2) == SET);   // KEY32 - PD2
        case 32: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_8) == SET);   // KEY33 - PC8
        case 33: return (gpio_input_data_bit_read(GPIOA, GPIO_PINS_0) == SET);   // KEY34 - PA0
        case 34: return (gpio_input_data_bit_read(GPIOB, GPIO_PINS_3) == SET);   // KEY35 - PB3
        case 35: return (gpio_input_data_bit_read(GPIOC, GPIO_PINS_9) == SET);   // KEY36 - PC9
        default: break;
    }
    return 0; 
}
