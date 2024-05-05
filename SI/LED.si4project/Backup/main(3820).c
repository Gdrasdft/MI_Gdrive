/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

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
#include "arm_math.h"
#include <stdio.h>
#include "main.h"

void led_spark(void);
void DRV8323_Init_ResultPrint(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
    /* configure systick */
    systick_config();
    gd_eval_led_init(LED3);
	gd_eval_com_init(EVAL_COM1);
	//gd_eval_dma_init();
	//gd_eval_adc_init(ADC_VOL);
	spi_config();
	spi1_config();
	
	delay_1ms(50);
	DRV8323_Init_ResultPrint();
	timer_config();
	MI_FOC_initialize();
    while (1)
	{
		//drv8323rs_data = Drv8323_ReadData(Fault_Status1);
		//angle_phy = (float)(AS5047_GetAngle())/ANGLE_DIGITAL*ANGLE_CYCLE;
		//printf("\r\n CURRENT FaultStatus = %d", drv8323rs_data);
        //printf("\r\n CURRENT ANGLE = %.3f", angle_phy);
        /*timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,pulse-1);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,0);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,0);
		delay_1ms(Time);
		
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,pulse/2-1);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,pulse/2-1);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,0);
		delay_1ms(Time);
		
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,0);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,pulse/2-1);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,0);
		delay_1ms(Time);
		
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,0);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,pulse/2-1);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,pulse/2-1);
		delay_1ms(Time);
		
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,0);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,0);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,pulse-1);
		delay_1ms(Time);
		
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,pulse/2-1);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,0);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,pulse/2-1);
		delay_1ms(Time);*/
		printf("ThreePhase: %f,%f,%f,%f,%f,%f,%f\n",SVPWM_OutCmp.Tcmp1,SVPWM_OutCmp.Tcmp2,SVPWM_OutCmp.Tcmp3,ClarkeCur.Ialpha,ClarkeCur.Ibeta,Park_Cur.Id,Park_Cur.Iq);
	}
}
/*!
    \brief      toggle the led every 500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_spark(void)
{
    static __IO uint32_t timingdelaylocal = 0U;

    if(timingdelaylocal)
		{

        if(timingdelaylocal < 500U)
		{
            gd_eval_led_on(LED3);
        }
		else
		{
            gd_eval_led_off(LED3);
        }

        timingdelaylocal--;
    }
		else
		{
        timingdelaylocal = 1000U;
    }
}

void DRV8323_Init_ResultPrint(void)
{
	uint16_t Result;
	Result = DRV8323_Init_Device();
	switch(Result)
	{
		case 1: 
		printf("\r\n Init Drive_Control_Reg FAIL...");
		break;
		case 2: 
		printf("\r\n Init Gate_DriveHS_Reg FAIL...");
		break;
		case 3: 
		printf("\r\n Init Gate_DriveLS_Reg FAIL...");
		break;
		case 4: 
		printf("\r\n Init OCP_Control_Reg FAIL...");
		break;
		case 5: 
		printf("\r\n Init CSA_Control_Reg FAIL...");
		break;
		default:
		printf("\r\n DRV8323RS Init Success!");
	}

}
void Report_Drv8323_FaultInfo(void)
{
	static uint32_t Last_FaultInfo;
	Drv8323_FaultInfo = (((uint32_t)Drv8323_ReadData(Fault_Status1))<<11) | (((uint32_t)Drv8323_ReadData(Fault_Status2))<<0);
	Current_FaultInfo = Drv8323_FaultInfo - Last_FaultInfo;
	for(int i=0;i<21;i++)
	{
		if(Current_FaultInfo == BIT(i))
		{
			ErrCode = i;
			printf("\r\n ErrCode is %d",ErrCode);
			return;
		}
	}
}
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM1, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));

    return ch;
}

