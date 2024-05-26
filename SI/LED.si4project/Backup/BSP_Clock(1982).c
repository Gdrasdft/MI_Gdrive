#include "BSP_Clock.h"

void clock_config(void)
{
    /* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
	/* enable alternate function clock */
	rcu_periph_clock_enable(RCU_AF);
	/* enable SPI2 clock */
	rcu_periph_clock_enable(RCU_SPI2);
	/* enable SPI1 clock */
	rcu_periph_clock_enable(RCU_SPI1);
	/* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
	/* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);
	/* enable TIMER0 clock */
	rcu_periph_clock_enable(RCU_TIMER0);
}


