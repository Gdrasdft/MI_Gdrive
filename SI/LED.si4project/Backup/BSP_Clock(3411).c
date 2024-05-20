#include "BSP_Clock.h"

void clock_config(void)
{
    /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
}
