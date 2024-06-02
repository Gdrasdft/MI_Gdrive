#include "BSP_MotorPwm.h"

uint8_t TimerCouner_Dir = 0;
uint32_t UpCountNum = 0;
uint32_t DowmCountNum = 0;

timer_oc_parameter_struct timer_ocintpara;
timer_parameter_struct timer_initpara;


void timer_config(void)
{
/* -----------------------------------------------------------------------
    TIMER0 configuration to:
    generate 3 complementary PWM signals with 3 different duty cycles:
    TIMER0CLK is fixed to systemcoreclock, the TIMER0 prescaler is equal to 6000 so the 
    TIMER0 counter clock used is 20KHz.
    the three duty cycles are computed as the following description: 
    the channel 0 duty cycle is set to 25% so channel 1N is set to 75%.
    the channel 1 duty cycle is set to 50% so channel 2N is set to 50%.
    the channel 2 duty cycle is set to 75% so channel 3N is set to 25%.
  ----------------------------------------------------------------------- */
    timer_deinit(TIMER0);

    /* TIMER0 configuration */
    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_CENTER_BOTH;
    //timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	if(timer_initpara.alignedmode != TIMER_COUNTER_EDGE)
	{
		timer_initpara.period = (fTIMER_CK/((timer_initpara.clockdivision+1)*(timer_initpara.prescaler+1))/INIT_FREQ/2)-1;
	}
	else
	{
		timer_initpara.period = (fTIMER_CK/((timer_initpara.clockdivision+1)*(timer_initpara.prescaler+1))/INIT_FREQ)-1;
	}
	timer_update_source_config(TIMER0,TIMER_UPDATE_SRC_GLOBAL);
    timer_init(TIMER0,&timer_initpara);
	
	timer_channel_output_struct_para_init(&timer_ocintpara);
     /* CH1,CH2 and CH3 configuration in PWM mode */
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;


    timer_channel_output_config(TIMER0,TIMER_CH_0,&timer_ocintpara);
    timer_channel_output_config(TIMER0,TIMER_CH_1,&timer_ocintpara);
    timer_channel_output_config(TIMER0,TIMER_CH_2,&timer_ocintpara);
	timer_channel_output_config(TIMER0,TIMER_CH_3,&timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,499);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);

    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,499);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_1,TIMER_OC_SHADOW_ENABLE);

    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,499);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_2,TIMER_OC_SHADOW_ENABLE);

    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_3,timer_initpara.period);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_3,TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_3,TIMER_OC_SHADOW_ENABLE);

    timer_primary_output_config(TIMER0,ENABLE);

	timer_interrupt_flag_clear(TIMER0,TIMER_INTF_UPIF);
	timer_interrupt_enable(TIMER0,TIMER_INTF_UPIF);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER0);
    timer_enable(TIMER0);

}


