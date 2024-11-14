#include <at32f4xx.h>

void bsp_led_gpio_init()
{
	GPIO_InitType GPIO_InitStructure;
	/*Enable the LED Clock*/
	RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_GPIOF, ENABLE);

	/*Configure the LED pin as ouput push-pull*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_6;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OutType = GPIO_OutType_PP;
	GPIO_InitStructure.GPIO_Pull = GPIO_Pull_NOPULL;

	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;	
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

void bsp_led_turn_on(uint8_t id)
{
	GPIOF->BRE = GPIO_Pins_6;
}

void bsp_led_turn_off(uint8_t id)
{
 	GPIOF->BSRE = GPIO_Pins_6;
}

void bsp_led_toggle(uint8_t id)
{
 	GPIOF->OPTDT ^= GPIO_Pins_6;
}

	
