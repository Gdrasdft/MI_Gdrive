/*!
    \file    systick.c
    \brief   the systick configuration file

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x 
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x.h"
#include "systick.h"

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000U)){
        /* capture error */
        while (1){
        }
    }

}

/*!
    \brief      delay_1ms
    \param[in]  timecnt_ms
    \param[out] none
    \retval     none
*/

void delay_1ms(uint32_t Timecnt_ms)
{
	uint32_t delay = SysTick->VAL;
	while((SysTick->VAL - delay) <= (Timecnt_ms*(SystemCoreClock / 1000U))){}
}

/*!
    \brief      delay_1us
    \param[in]  timecnt_us
    \param[out] none
    \retval     none
*/

void delay_1us(uint32_t Timecnt_us)
{
	uint32_t delay = SysTick->VAL;
	while((SysTick->VAL - delay) <= (Timecnt_us*(SystemCoreClock / 1000000U))){}
}



/*!
    \brief      configure NVIC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NVIC_Config(void)
{
	/* pre-emption priority: 0 BIT*/
	/* subpriority: 4 BIT*/
	nvic_priority_group_set(NVIC_PRIGROUP_PRE0_SUB4);
	
	/* configure the EXTI handler priority */
    NVIC_SetPriority(EXTI10_15_IRQn, 0x00U);


	/* configure the ADC handler priority */
    NVIC_SetPriority(ADC0_1_IRQn, 0x01U);

	/* configure the TIMER0 Update handler priority */
    NVIC_SetPriority(TIMER0_UP_IRQn, 0x01U);

}

void System_Interrup_Enable(void)
{
	/* enable EXTI interrupt */
    NVIC_EnableIRQ(EXTI10_15_IRQn);

	/* enable ADC interrupt */
    NVIC_EnableIRQ(ADC0_1_IRQn);

	/* enable TIMER0 Update interrupt */
    NVIC_EnableIRQ(TIMER0_UP_IRQn);

}


