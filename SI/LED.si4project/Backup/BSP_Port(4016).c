#include "BSP_Port.h"


void Port_Config(void)
{
    /* configure led GPIO port */ 
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_13);
}




