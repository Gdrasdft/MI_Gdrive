/*!
    \file    gd32f307c_eval.c
    \brief   firmware functions to manage leds, keys, COM ports

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

#include <gd32f30x.h>
#include "gd32f307c_eval.h"

/* private variables */
uint16_t adc_value[4];

static uint32_t GPIO_PORT[LEDn]                = {LED2_GPIO_PORT, LED3_GPIO_PORT,
                                                  LED4_GPIO_PORT, LED5_GPIO_PORT};
static uint32_t GPIO_PIN[LEDn]                 = {LED2_PIN, LED3_PIN, LED4_PIN, LED5_PIN};
static rcu_periph_enum GPIO_CLK[LEDn]          = {LED2_GPIO_CLK, LED3_GPIO_CLK, 
                                                  LED4_GPIO_CLK, LED5_GPIO_CLK};


static rcu_periph_enum COM_CLK[COMn]           = {EVAL_COM0_CLK, EVAL_COM1_CLK};
static uint32_t COM_TX_PIN[COMn]               = {EVAL_COM0_TX_PIN, EVAL_COM1_TX_PIN};
static uint32_t COM_RX_PIN[COMn]               = {EVAL_COM0_RX_PIN, EVAL_COM1_RX_PIN};
static uint32_t COM_GPIO_PORT[COMn]            = {EVAL_COM0_GPIO_PORT, EVAL_COM1_GPIO_PORT};
static rcu_periph_enum COM_GPIO_CLK[COMn]      = {EVAL_COM0_GPIO_CLK, EVAL_COM1_GPIO_CLK};

static uint32_t ADC_012[ADCn]                  = {EVAL_VOL_ADC, EVAL_NTCP_ADC, EVAL_NTCN_ADC};
static uint8_t  ADC_CH[ADCn]                   = {EVAL_VOL_ADC_CH, EVAL_NTCP_ADC_CH, EVAL_NTCN_ADC_CH};
static uint8_t  ADC_ORDER[ADCn]                = {EVAL_VOL_ADC_ORDER, EVAL_NTCP_ADC_ORDER, EVAL_NTCN_ADC_ORDER};
static rcu_periph_enum ADC_CLK[ADCn]           = {EVAL_VOL_ADC_CLK, EVAL_NTCP_ADC_CLK, EVAL_NTCN_ADC_CLK};
static uint32_t ADC_PIN[ADCn]                  = {EVAL_VOL_ADC_PIN, EVAL_NTCP_ADC_PIN, EVAL_NTCN_ADC_PIN};
static uint32_t ADC_GPIO_PORT[ADCn]            = {EVAL_VOL_ADC_GPIO_PORT, EVAL_NTCP_ADC_GPIO_PORT, EVAL_NTCN_ADC_GPIO_PORT};
static rcu_periph_enum ADC_GPIO_CLK[ADCn]      = {EVAL_VOL_ADC_GPIO_CLK, EVAL_NTCP_ADC_GPIO_CLK, EVAL_NTCN_ADC_GPIO_CLK};


static uint32_t KEY_PORT[KEYn]                 = {WAKEUP_KEY_GPIO_PORT, 
                                                  TAMPER_KEY_GPIO_PORT,
                                                  USER_KEY_GPIO_PORT};
static uint32_t KEY_PIN[KEYn]                  = {WAKEUP_KEY_PIN, TAMPER_KEY_PIN,USER_KEY_PIN,};
static rcu_periph_enum KEY_CLK[KEYn]           = {WAKEUP_KEY_GPIO_CLK, 
                                                  TAMPER_KEY_GPIO_CLK,
                                                  USER_KEY_GPIO_CLK};
static exti_line_enum KEY_EXTI_LINE[KEYn]      = {WAKEUP_KEY_EXTI_LINE,
                                                  TAMPER_KEY_EXTI_LINE,
                                                  USER_KEY_EXTI_LINE};
static uint8_t KEY_PORT_SOURCE[KEYn]           = {WAKEUP_KEY_EXTI_PORT_SOURCE,
                                                  TAMPER_KEY_EXTI_PORT_SOURCE,
                                                  USER_KEY_EXTI_PORT_SOURCE};
static uint8_t KEY_PIN_SOURCE[KEYn]            = {WAKEUP_KEY_EXTI_PIN_SOURCE,
                                                  TAMPER_KEY_EXTI_PIN_SOURCE,
                                                  USER_KEY_EXTI_PIN_SOURCE};
static uint8_t KEY_IRQn[KEYn]                  = {WAKEUP_KEY_EXTI_IRQn, 
                                                  TAMPER_KEY_EXTI_IRQn,
                                                  USER_KEY_EXTI_IRQn};

/*!
    \brief      configure led GPIO
    \param[in]  lednum: specify the led to be configured
      \arg        LED2
      \arg        LED3
      \arg        LED4
      \arg        LED5
    \param[out] none
    \retval     none
*/
void  gd_eval_led_init (led_typedef_enum lednum)
{
    /* enable the led clock */
    rcu_periph_clock_enable(GPIO_CLK[lednum]);
    /* configure led GPIO port */ 
    gpio_init(GPIO_PORT[lednum], GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ,GPIO_PIN[lednum]);

    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn on selected led
    \param[in]  lednum: specify the led to be turned on
      \arg        LED2
      \arg        LED3
      \arg        LED4
      \arg        LED5
    \param[out] none
    \retval     none
*/
void gd_eval_led_on(led_typedef_enum lednum)
{
    GPIO_BOP(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn off selected led
    \param[in]  lednum: specify the led to be turned off
      \arg        LED2
      \arg        LED3
      \arg        LED4
      \arg        LED5
    \param[out] none
    \retval     none
*/
void gd_eval_led_off(led_typedef_enum lednum)
{
    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      toggle selected led
    \param[in]  lednum: specify the led to be toggled
      \arg        LED2
      \arg        LED3
      \arg        LED4
      \arg        LED5
    \param[out] none
    \retval     none
*/
void gd_eval_led_toggle(led_typedef_enum lednum)
{
    gpio_bit_write(GPIO_PORT[lednum], GPIO_PIN[lednum], 
                    (bit_status)(1-gpio_input_bit_get(GPIO_PORT[lednum], GPIO_PIN[lednum])));
}

/*!
    \brief      configure key
    \param[in]  key_num: specify the key to be configured
      \arg        KEY_TAMPER: tamper key
      \arg        KEY_WAKEUP: wakeup key
      \arg        KEY_USER: user key
    \param[in]  key_mode: specify button mode
      \arg        KEY_MODE_GPIO: key will be used as simple IO
      \arg        KEY_MODE_EXTI: key will be connected to EXTI line with interrupt
    \param[out] none
    \retval     none
*/
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode)
{
    /* enable the key clock */
    rcu_periph_clock_enable(KEY_CLK[key_num]);
    rcu_periph_clock_enable(RCU_AF);

    /* configure button pin as input */
    gpio_init(KEY_PORT[key_num], GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, KEY_PIN[key_num]);

    if (key_mode == KEY_MODE_EXTI) {
        /* enable and set key EXTI interrupt to the lowest priority */
        nvic_irq_enable(KEY_IRQn[key_num], 2U, 0U);

        /* connect key EXTI line to key GPIO pin */
        gpio_exti_source_select(KEY_PORT_SOURCE[key_num], KEY_PIN_SOURCE[key_num]);

        /* configure key EXTI line */
        exti_init(KEY_EXTI_LINE[key_num], EXTI_INTERRUPT, EXTI_TRIG_FALLING);
        exti_interrupt_flag_clear(KEY_EXTI_LINE[key_num]);
    }
}

/*!
    \brief      return the selected key state
    \param[in]  key: specify the key to be checked
      \arg        KEY_TAMPER: tamper key
      \arg        KEY_WAKEUP: wakeup key
      \arg        KEY_USER: user key
    \param[out] none
    \retval     the key's GPIO pin value
*/
uint8_t gd_eval_key_state_get(key_typedef_enum key)
{
    return gpio_input_bit_get(KEY_PORT[key], KEY_PIN[key]);
}

/*!
    \brief      configure COM port
    \param[in]  com: COM on the board
      \arg        EVAL_COM0: COM0 on the board
      \arg        EVAL_COM1: COM1 on the board
    \param[out] none
    \retval     none
*/
void gd_eval_com_init(uint32_t com)
{
    uint32_t com_id = 0U;
    if(EVAL_COM0 == com){
        com_id = 0U;
    }else if(EVAL_COM1 == com){
        com_id = 1U;
    }
    
    /* enable GPIO clock */
    rcu_periph_clock_enable(COM_GPIO_CLK[com_id]);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[com_id]);

    /* connect port to USARTx_Tx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, COM_TX_PIN[com_id]);

    /* connect port to USARTx_Rx */
    gpio_init(COM_GPIO_PORT[com_id], GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, COM_RX_PIN[com_id]);

    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, 115200U);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
    usart_enable(com);
}

void gd_eval_adc_init(adc_typedef_enum adc)
{
	rcu_periph_clock_enable(ADC_GPIO_CLK[adc]);
	rcu_periph_clock_enable(ADC_CLK[adc]);
	rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
	gpio_init(ADC_GPIO_PORT[adc], GPIO_MODE_AIN, GPIO_OSPEED_10MHZ, ADC_PIN[adc]);
	
    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE); 
    /* ADC continuous function enable */
    adc_special_function_config(ADC_012[adc], ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC scan function enable */
    adc_special_function_config(ADC_012[adc], ADC_SCAN_MODE, ENABLE);
	/* ADC data resolution config */
	adc_resolution_config(ADC_012[adc],ADC_RESOLUTION_12B);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC_012[adc], ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */
    adc_channel_length_config(ADC_012[adc], ADC_REGULAR_CHANNEL, 3);

    /* ADC regular channel config */
	for(int i = 0; i < 3; i++)
    {
    	adc_regular_channel_config(ADC_012[adc+i], ADC_ORDER[adc+i] , ADC_CH[adc+i], ADC_SAMPLETIME_55POINT5);
	}
    
    /* ADC trigger config */
    adc_external_trigger_source_config(ADC_012[adc], ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE);
    adc_external_trigger_config(ADC_012[adc], ADC_REGULAR_CHANNEL, ENABLE);

    /* ADC DMA function enable */
    adc_dma_mode_enable(ADC_012[adc]);
    /* enable ADC interface */
    adc_enable(ADC_012[adc]);
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC_012[adc]);

    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC_012[adc], ADC_REGULAR_CHANNEL);

}

void gd_eval_dma_init(void)
{
	rcu_periph_clock_enable(RCU_DMA0);
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
    
    /* ADC DMA_channel configuration */
    dma_deinit(DMA0, DMA_CH0);
    
    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr  = (uint32_t)(&adc_value);
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;  
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number       = 3;
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);

    dma_circulation_enable(DMA0, DMA_CH0);
  
    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);

	
}

