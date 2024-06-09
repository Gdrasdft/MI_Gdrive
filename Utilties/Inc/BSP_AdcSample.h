#ifndef BSP_ADC_SAMPLE_H
#define BSP_ADC_SAMPLE_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "gd32f30x.h"

/* ADC for Motor Temp and BUS_VOL */

#define ADC_INJ_CH_NUM                  2
#define ADC_REG_CH_NUM                  2

#define MI_FOC_ADC0                      ADC0
#define MI_FOC_ADC0_CLK                  RCU_ADC0

#define MI_FOC_ADC1                      ADC1
#define MI_FOC_ADC1_CLK                  RCU_ADC1


#define MIFOC_SOB_ADC_CH                 ADC_CHANNEL_11
//#define MIFOC_SOB_ADC_CH                 ADC_CHANNEL_5
#define MIFOC_SOB_ADC_ORDER              0
#define MIFOC_SOB_ADC_PIN                GPIO_PIN_1
#define MIFOC_SOB_ADC_GPIO_PORT          GPIOC
#define MIFOC_SOB_ADC_RISTOR             0.001F

#define MIFOC_VOL_ADC_CH                 ADC_CHANNEL_13
//#define MIFOC_VOL_ADC_CH                 ADC_CHANNEL_6
#define MIFOC_VOL_ADC_ORDER              1
#define MIFOC_VOL_ADC_PIN                GPIO_PIN_3
#define MIFOC_VOL_ADC_GPIO_PORT          GPIOC
#define MIFOC_VOL_ADC_FACTOR             16


#define MIFOC_SOC_ADC_CH                 ADC_CHANNEL_10
//#define MIFOC_SOC_ADC_CH                 ADC_CHANNEL_7
#define MIFOC_SOC_ADC_ORDER              0
#define MIFOC_SOC_ADC_PIN                GPIO_PIN_0
#define MIFOC_SOC_ADC_GPIO_PORT          GPIOC
#define MIFOC_SOC_ADC_RISTOR             0.001F


#define MIFOC_SOA_ADC_CH                 ADC_CHANNEL_14
#define MIFOC_SOA_ADC_ORDER              1
#define MIFOC_SOA_ADC_PIN                GPIO_PIN_4
#define MIFOC_SOA_ADC_GPIO_PORT          GPIOC


#define MIFOC_NTC1_ADC_CH                ADC_CHANNEL_12
#define MIFOC_NTC1_ADC_ORDER             0
#define MIFOC_NTC1_ADC_PIN               GPIO_PIN_2
#define MIFOC_NTC1_ADC_GPIO_PORT         GPIOC


#define MIFOC_NTC2_ADC_CH                ADC_CHANNEL_8
#define MIFOC_NTC2_ADC_ORDER             1
#define MIFOC_NTC2_ADC_PIN               GPIO_PIN_0
#define MIFOC_NTC2_ADC_GPIO_PORT         GPIOB


#define WDG_HIGH_THRESHOLD               3500
#define WDG_LOW_THRESHOLD                100

#define ADC_REG_RANGE                    4095.0F
#define ADC_VOL_REFER                    3.3F
#define ADC_VOL_HALREFER                 1.65F

extern uint16_t adc_value[4];
extern uint16_t FOC_ADC_REG[3];
extern float Stator_Temp;
extern float PCB_Temp;

#define NTC_LTB 1
#define NTC_KELVIN 2
#define GET_NTCRES_MOTHD NTC_KELVIN

typedef struct NTC_TEMP
{
	uint16_t ShortAD_Value;
	uint16_t BreakAD_Value;
#if GET_NTCRES_MOTHD == NTC_LTB
	const uint16_t* LTB_Array;
	int16_t StartTemp;
	uint16_t TempStep;
#endif
#if GET_NTCRES_MOTHD == NTC_KELVIN
	uint16_t Factor_B;
	uint16_t DivdR;
	uint16_t RateR;
	float RateTemp;
	float RateKelVin;
	float ErrComp;
#endif
}NtcTemp;


void adc_config(void);
void User_SofeTrig_RegularGroupConver(void);
void NTC_Config_DataInit(void);
float GET_NTC_TEMP(uint16_t Reg_Value);


#ifdef __cplusplus
}
#endif

#endif



