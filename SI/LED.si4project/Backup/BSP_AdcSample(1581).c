#include "BSP_AdcSample.h"
#include "systick.h"
uint16_t adc_value[4];

void adc_dma_init(void)
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

/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
   /* enable ADC0 clock */
    rcu_periph_clock_enable(MI_FOC_ADC_CLK);
    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
	
    /* config the GPIO as analog mode */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_init(MIFOC_VOL_ADC_GPIO_PORT, GPIO_MODE_AIN, GPIO_OSPEED_MAX, MIFOC_VOL_ADC_PIN);
	gpio_init(MIFOC_NTC1_ADC_GPIO_PORT, GPIO_MODE_AIN, GPIO_OSPEED_MAX, MIFOC_NTC1_ADC_PIN);
	gpio_init(MIFOC_NTC2_ADC_GPIO_PORT, GPIO_MODE_AIN, GPIO_OSPEED_MAX, MIFOC_NTC2_ADC_PIN);

    /* ADC mode config */
    adc_mode_config(ADC_MODE_FREE); 
    /* ADC special function config */
    adc_special_function_config(MI_FOC_ADC, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(MI_FOC_ADC, ADC_CONTINUOUS_MODE, DISABLE);  
    /* ADC data alignment config */
    adc_data_alignment_config(MI_FOC_ADC, ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */
    adc_channel_length_config(MI_FOC_ADC, ADC_INSERTED_CHANNEL, ADC_SAMP_CH_NUM);
    /* ADC inserted channel config */
    adc_inserted_channel_config(MI_FOC_ADC, MIFOC_VOL_ADC_ORDER,  MIFOC_VOL_ADC_CH,  ADC_SAMPLETIME_55POINT5);
    adc_inserted_channel_config(MI_FOC_ADC, MIFOC_NTC1_ADC_ORDER, MIFOC_NTC1_ADC_CH, ADC_SAMPLETIME_55POINT5);
    adc_inserted_channel_config(MI_FOC_ADC, MIFOC_NTC2_ADC_ORDER, MIFOC_NTC2_ADC_CH, ADC_SAMPLETIME_55POINT5);
    //adc_inserted_channel_config(MI_FOC_ADC, 3, ADC_CHANNEL_3, ADC_SAMPLETIME_55POINT5);
    /* ADC trigger config */
    adc_external_trigger_source_config(MI_FOC_ADC, ADC_INSERTED_CHANNEL, ADC0_1_EXTTRIG_INSERTED_T1_CH0); 
    /* ADC external trigger enable */
    adc_external_trigger_config(MI_FOC_ADC, ADC_INSERTED_CHANNEL, ENABLE);
    
    /* clear the ADC flag */
    adc_interrupt_flag_clear(MI_FOC_ADC, ADC_INT_FLAG_EOC);
    adc_interrupt_flag_clear(MI_FOC_ADC, ADC_INT_FLAG_EOIC);
    /* enable ADC interrupt */
	//nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(ADC0_1_IRQn, 1, 0);
    adc_interrupt_enable(MI_FOC_ADC, ADC_INT_EOIC);
	

    /* enable ADC interface */
    adc_enable(MI_FOC_ADC);
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(MI_FOC_ADC);
	/* ADC DMA Enable */
	adc_dma_init();
}

