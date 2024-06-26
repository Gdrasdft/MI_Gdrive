#include "BSP_AdcSample.h"
#include "systick.h"
#if GET_NTCRES_MOTHD == NTC_KELVIN
#include <math.h>
#endif

uint16_t adc_value[4];
uint16_t FOC_ADC_REG[3];
float Stator_Temp;
float PCB_Temp;

#if GET_NTCRES_MOTHD == NTC_LTB
const uint16_t NTC3470_LT[] = {
    3072, 3037, 3002, 2966, 2930, 2894, 2857, 2819, 2782, 2743,
    2705, 2666, 2627, 2588, 2548, 2509, 2469, 2429, 2389, 2349,
    2310, 2270, 2230, 2190, 2150, 2111, 2072, 2032, 1993, 1955,
    1916, 1878, 1841, 1803, 1766, 1730, 1693, 1658, 1622, 1587,
    1553, 1519, 1486, 1453, 1420, 1388, 1357, 1326, 1295, 1265,
    1236, 1207, 1179, 1151, 1124, 1098, 1071, 1046, 1021, 996,
    972,  949,  926,  904,  882,  860,  839,  819,  799,  779, 
    760,  741,  723,  705,  688,  671,  655,  639,  623,  608, 
    593,  578,  564,  550,  537,  524,  511,  498,  486,  474, 
    463,  452,  441,  430,  420,  410,  400,  390,  381,  372
};
uint16_t NTC_TABLE_NUM = sizeof(NTC3470_LT)/sizeof(NTC3470_LT[0]);
#endif

NtcTemp NtcTemp_DataInfo;

void NTC_Config_DataInit(void)
{
	NtcTemp_DataInfo.ShortAD_Value = 300;
	NtcTemp_DataInfo.BreakAD_Value = 4500;
	
#if GET_NTCRES_MOTHD == NTC_LTB
	NtcTemp_DataInfo.LTB_Array = NTC3470_LT;
	NtcTemp_DataInfo.StartTemp = 0;
	NtcTemp_DataInfo.TempStep = 1;
#endif

#if GET_NTCRES_MOTHD == NTC_KELVIN
	NtcTemp_DataInfo.Factor_B = 3470U;
	NtcTemp_DataInfo.DivdR = 4700U;
	NtcTemp_DataInfo.RateR = 5000U;
	NtcTemp_DataInfo.RateTemp = 25.0F;
	NtcTemp_DataInfo.RateKelVin = 273.15F;
#endif
}

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


float GET_NTC_TEMP(uint16_t Reg_Value)
{
	float temp_result = 0.0F;
	if(Reg_Value <= NtcTemp_DataInfo.ShortAD_Value)
	{
		temp_result = 140.0F;
		return temp_result;
	}
	else if(Reg_Value >= NtcTemp_DataInfo.BreakAD_Value)
	{
		temp_result = 0.0F;
		return temp_result;
	}
	else
	{
#if GET_NTCRES_MOTHD == NTC_LTB
		uint16_t Index_Left = 0;
		uint16_t Index_Right = NTC_TABLE_NUM-1;
		uint16_t mid = 0;
		float temp_mid = 0;
		float temp_step = 0;
		while(Index_Left < Index_Right)
		{
			mid = (Index_Left + Index_Right) >> 1;
			if(Reg_Value == NtcTemp_DataInfo.LTB_Array[mid])
			{
				temp_result = (float)(NtcTemp_DataInfo.StartTemp + NtcTemp_DataInfo.TempStep*mid);
				return temp_result;
			}
			else if(Reg_Value < NTC3470_LT[mid])
			{
				Index_Left = mid+1;
			}
			else
			{
				Index_Right = mid-1;
			}
		}
		temp_mid = (NtcTemp_DataInfo.StartTemp+NtcTemp_DataInfo.TempStep*mid);
		temp_step = ((float)(NtcTemp_DataInfo.LTB_Array[mid]-Reg_Value))/(NtcTemp_DataInfo.LTB_Array[mid]-NtcTemp_DataInfo.LTB_Array[mid+1])*(float)NtcTemp_DataInfo.TempStep;
		temp_result = temp_mid + temp_step;
		return temp_result;
#endif
		
#if GET_NTCRES_MOTHD == NTC_KELVIN
		float B_value  = NtcTemp_DataInfo.Factor_B;
		float Rp_value = NtcTemp_DataInfo.RateR;
		float T1 = 0.0F;
		float T2 = NtcTemp_DataInfo.RateKelVin+NtcTemp_DataInfo.RateTemp;
		float DivFac = ((float)Reg_Value)/ADC_REG_RANGE;
		float Rt_value = DivFac/(1-DivFac)*((float)NtcTemp_DataInfo.DivdR);
		T1 = (1.0F / ((log(Rt_value/Rp_value) / B_value) + (1.0F/T2)))-NtcTemp_DataInfo.RateKelVin;
		return T1;
#endif
		
	}
}


