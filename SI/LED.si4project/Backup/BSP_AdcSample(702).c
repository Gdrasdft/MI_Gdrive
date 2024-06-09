#include "BSP_AdcSample.h"
#include "systick.h"
uint16_t adc_value[4];
uint16_t FOC_ADC_REG[3];

void adc_dma_init(void)
{
	/* ADC DMA Enable */
	adc_dma_mode_enable(MI_FOC_ADC0);

	/* ADC DMA_channel configuration */
    dma_deinit(DMA0, DMA_CH0);
	
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
        
    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr  = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr  = (uint32_t)(&adc_value);
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;  
    dma_data_parameter.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number       = 2;
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);
	
	/* enable DMA circle Transfer */
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
    /* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);

    /* ADC mode config */
    adc_mode_config(ADC_DAUL_INSERTED_PARALLEL); 
    /* ADC special function config */
    adc_special_function_config(MI_FOC_ADC0, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(MI_FOC_ADC0, ADC_INSERTED_CHANNEL_AUTO, DISABLE);
	adc_special_function_config(MI_FOC_ADC0, ADC_CONTINUOUS_MODE, DISABLE);
	adc_special_function_config(MI_FOC_ADC1, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(MI_FOC_ADC1, ADC_INSERTED_CHANNEL_AUTO, DISABLE);
	adc_special_function_config(MI_FOC_ADC1, ADC_CONTINUOUS_MODE, DISABLE);
	
    /* ADC data alignment config */
    adc_data_alignment_config(MI_FOC_ADC0, ADC_DATAALIGN_RIGHT);
	adc_data_alignment_config(MI_FOC_ADC1, ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */
    adc_channel_length_config(MI_FOC_ADC0, ADC_INSERTED_CHANNEL, ADC_INJ_CH_NUM);
	adc_channel_length_config(MI_FOC_ADC1, ADC_INSERTED_CHANNEL, ADC_INJ_CH_NUM);
	adc_channel_length_config(MI_FOC_ADC0, ADC_REGULAR_CHANNEL, ADC_REG_CH_NUM);
	
    /* ADC injected channel config */
    adc_inserted_channel_config(MI_FOC_ADC0, MIFOC_SOB_ADC_ORDER,  MIFOC_SOB_ADC_CH,  ADC_SAMPLETIME_7POINT5);
    adc_inserted_channel_config(MI_FOC_ADC0, MIFOC_VOL_ADC_ORDER,  MIFOC_VOL_ADC_CH,  ADC_SAMPLETIME_7POINT5);
	adc_inserted_channel_config(MI_FOC_ADC1, MIFOC_SOC_ADC_ORDER,  MIFOC_SOC_ADC_CH,  ADC_SAMPLETIME_7POINT5);
	adc_inserted_channel_config(MI_FOC_ADC1, MIFOC_SOA_ADC_ORDER,  MIFOC_SOA_ADC_CH,  ADC_SAMPLETIME_7POINT5);
	
	/* ADC regular channel config */
	adc_regular_channel_config(MI_FOC_ADC0, MIFOC_NTC1_ADC_ORDER, MIFOC_NTC1_ADC_CH, ADC_SAMPLETIME_7POINT5);
	adc_regular_channel_config(MI_FOC_ADC0, MIFOC_NTC2_ADC_ORDER, MIFOC_NTC2_ADC_CH, ADC_SAMPLETIME_7POINT5);

    /* ADC Inject trigger config */
    adc_external_trigger_source_config(MI_FOC_ADC0, ADC_INSERTED_CHANNEL, ADC0_1_EXTTRIG_INSERTED_T0_CH3); 
	adc_external_trigger_source_config(MI_FOC_ADC1, ADC_INSERTED_CHANNEL, ADC0_1_EXTTRIG_INSERTED_T0_CH3);
	
    /* ADC Inject external trigger enable */
    adc_external_trigger_config(MI_FOC_ADC0, ADC_INSERTED_CHANNEL, ENABLE);
	adc_external_trigger_config(MI_FOC_ADC1, ADC_INSERTED_CHANNEL, ENABLE);
	
	/* ADC Regualr trigger config */
    adc_external_trigger_source_config(MI_FOC_ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); 
	/* ADC Regular external trigger enable */
    adc_external_trigger_config(MI_FOC_ADC0, ADC_REGULAR_CHANNEL, ENABLE);

	/* ADC Watchdog config */
	adc_watchdog_threshold_config(MI_FOC_ADC0, WDG_LOW_THRESHOLD, WDG_HIGH_THRESHOLD);
	adc_watchdog_single_channel_enable(MI_FOC_ADC0,MIFOC_SOB_ADC_CH);


	adc_watchdog_threshold_config(MI_FOC_ADC1, WDG_LOW_THRESHOLD, WDG_HIGH_THRESHOLD);
	adc_watchdog_single_channel_enable(MI_FOC_ADC1,MIFOC_SOC_ADC_CH);


	/* clear the ADC flag */
    adc_interrupt_flag_clear(MI_FOC_ADC0, ADC_INT_FLAG_WDE);
	adc_interrupt_flag_clear(MI_FOC_ADC1, ADC_INT_FLAG_WDE);
    adc_interrupt_flag_clear(MI_FOC_ADC0, ADC_INT_FLAG_EOC);
	adc_interrupt_flag_clear(MI_FOC_ADC0, ADC_INT_FLAG_EOIC);
	adc_interrupt_enable(MI_FOC_ADC0, ADC_INT_WDE);
	adc_interrupt_enable(MI_FOC_ADC1, ADC_INT_WDE);
	adc_interrupt_enable(MI_FOC_ADC0, ADC_INT_EOIC);

    /* enable ADC interface */
    adc_enable(MI_FOC_ADC0);
	adc_enable(MI_FOC_ADC1);
    delay_1ms(1);
    /* ADC calibration and reset calibration */
	adc_calibration_enable(MI_FOC_ADC0);
    adc_calibration_enable(MI_FOC_ADC1);

	/* ADC DMA Enable --- Tranfer Regular Data Reg*/
	adc_dma_init();
	/* ADC Regular Soft Conversion Trigger */
	adc_software_trigger_enable(MI_FOC_ADC0,ADC_REGULAR_CHANNEL);
}


void User_SofeTrig_RegularGroupConver(void)
{
	if(adc_flag_get(MI_FOC_ADC0,ADC_FLAG_EOC) == SET)
	{
		adc_software_trigger_enable(MI_FOC_ADC0,ADC_REGULAR_CHANNEL);
		adc_flag_clear(MI_FOC_ADC0,ADC_FLAG_EOC);
	}
}


float ADC_Get_Resistor(uint16_t Reg_Value)
{
	float TempValue = 0;
	TempValue = (float)Reg_Value/ADC_REG_RANGE;
}

