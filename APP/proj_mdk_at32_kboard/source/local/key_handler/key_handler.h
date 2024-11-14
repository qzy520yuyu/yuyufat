#ifndef KEY_HANDLER_H
#define KEY_HANDLER_H

#include <stdint.h>
#include "type_kboard.h"

void convert_keys_to_notes();
void bias_adc_update();
void arpstate_adc();
void bsp_key_gpio_scan();
#endif 
