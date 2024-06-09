/*!
    \file    gd32f30x_it.c
    \brief   interrupt service routines

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

#include "gd32f30x_it.h"
#include "main.h"
#include "systick.h"

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
	//MI_FOC_step();
}

/*!
    \brief      this function handles external lines 10 to 15 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI10_15_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(FAULT_EXTI_LINE)) 
	{
		Report_Drv8323_FaultInfo();
        exti_interrupt_flag_clear(FAULT_EXTI_LINE);
    }
}

/*!
    \brief      this function handles TIMER0 Update interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void TIMER0_UP_IRQHandler(void)
{
	if(RESET != timer_flag_get(TIMER0,TIMER_INTF_UPIF))
	{
		TimerCouner_Dir = TIMER_CTL0(TIMER0)&TIMER_CTL0_DIR;
		timer_interrupt_flag_clear(TIMER0,TIMER_INTF_UPIF);
		//���ϼ���
		if(CNT_DIR_UP == TimerCouner_Dir)
		{
			UpCountNum++;
			GPIO_BOP(GPIOC) = GPIO_PIN_14;
			TIMER_CH3CV(TIMER0) = ADC_SAMPLE_DELAY_CNT;
			TIMER_CHCTL1(TIMER0) &= (~(uint32_t)TIMER_CHCTL1_CH3COMCTL);
        	TIMER_CHCTL1(TIMER0) |= (uint32_t)((uint32_t)(TIMER_OC_MODE_PWM1) << 8U);
		}
		//���¼���
		else
		{
			DowmCountNum++;
			GPIO_BC(GPIOC) = GPIO_PIN_14;
			TIMER_CH3CV(TIMER0) = timer_initpara.period - ADC_SAMPLE_DELAY_CNT;
			TIMER_CHCTL1(TIMER0) &= (~(uint32_t)TIMER_CHCTL1_CH3COMCTL);
        	TIMER_CHCTL1(TIMER0) |= (uint32_t)((uint32_t)(TIMER_OC_MODE_PWM0) << 8U);
		}
		//��ȡ�ű�����ת�ӽǶ�
		RtrAngle = (float)(AS5047_GetAngle())/ANGLE_DIGITAL*ANGLE_CYCLE;
	}
}

/*!
    \brief      this function handles ADC0_1 interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t MotorRun_Enable = 0;
float MotorRun_Freq = 1;

void ADC0_1_IRQHandler(void)
{
    /* Judge ADC IRQ TYPE */
	if(((ADC_STAT(MI_FOC_ADC0) & ADC_STAT_WDE)) || ((ADC_STAT(MI_FOC_ADC1) & ADC_STAT_WDE)))
	{
		//V W������ж�
		adc_interrupt_flag_clear(MI_FOC_ADC0, ADC_INT_FLAG_WDE);
		adc_interrupt_flag_clear(MI_FOC_ADC1, ADC_INT_FLAG_WDE);
	    adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOIC);
	}
	else
	{
		//����ͬ�����ʸ�������ж�
		adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOIC);
		if(CNT_DIR_UP == TimerCouner_Dir)
		{
			GPIO_BOP(GPIOC) = GPIO_PIN_13;
			FOC_ADC_REG[0] = (uint16_t)(ADC_IDATA1(ADC0)&ADC_IDATAX_IDATAN);
			FOC_ADC_REG[1] = (uint16_t)(ADC_IDATA0(ADC0)&ADC_IDATAX_IDATAN);
			FOC_ADC_REG[2] = (uint16_t)(ADC_IDATA0(ADC1)&ADC_IDATAX_IDATAN);
			GPIO_BC(GPIOC) = GPIO_PIN_13;
			FOC_Input_VolCur.Udc = (((float)FOC_ADC_REG[0])/ADC_REG_RANGE)*ADC_VOL_REFER*MIFOC_VOL_ADC_FACTOR;
			rtU.Ib = (0.5F-(((float)FOC_ADC_REG[1])/ADC_REG_RANGE))*ADC_VOL_REFER/20.0F/0.01F;
			rtU.Ic = (0.5F-(((float)FOC_ADC_REG[2])/ADC_REG_RANGE))*ADC_VOL_REFER/20.0F/0.01F;
			rtU.Ia = 0-rtU.Ib-rtU.Ic;
			if(MotorRun_Enable)
			{
				rtU.Theta += 2*3.14159265358979f/10000.0f*MotorRun_Freq;
				if(rtU.Theta >= 2*3.14159265358979f)
				{
					rtU.Theta = 0;
				}
				MI_FOC_step();
			}
			else
			{
				SVPWM_OutCmp.Tcmp1 = 0;
				SVPWM_OutCmp.Tcmp2 = 0;
				SVPWM_OutCmp.Tcmp3 = 0;
			}
			TIMER_CH0CV(TIMER0) = (uint32_t)SVPWM_OutCmp.Tcmp1;
			TIMER_CH1CV(TIMER0) = (uint32_t)SVPWM_OutCmp.Tcmp2;
			TIMER_CH2CV(TIMER0) = (uint32_t)SVPWM_OutCmp.Tcmp3;
			
		}
	}
}


