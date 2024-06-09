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
#define MIFOC_SOB_ADC_ORDER              0
#define MIFOC_SOB_ADC_PIN                GPIO_PIN_1
#define MIFOC_SOB_ADC_GPIO_PORT          GPIOC

#define MIFOC_VOL_ADC_CH                 ADC_CHANNEL_13
#define MIFOC_VOL_ADC_ORDER              1
#define MIFOC_VOL_ADC_PIN                GPIO_PIN_3
#define MIFOC_VOL_ADC_GPIO_PORT          GPIOC


#define MIFOC_SOC_ADC_CH                 ADC_CHANNEL_10
#define MIFOC_SOC_ADC_ORDER              0
#define MIFOC_SOC_ADC_PIN                GPIO_PIN_0
#define MIFOC_SOC_ADC_GPIO_PORT          GPIOC

#define MIFOC_SOA_ADC_CH                 ADC_CHANNEL_14
#define MIFOC_SOA_ADC_ORDER              1
#define MIFOC_SOA_ADC_PIN                GPIO_PIN_4
#define MIFOC_SOA_ADC_GPIO_PORT          GPIOC




#define MIFOC_NTC1_ADC_CH                ADC_CHANNEL_12
#define MIFOC_NTC1_ADC_ORDER             0
#define MIFOC_NTC1_ADC_PIN               GPIO_PIN_2
#define MIFOC_NTC1_ADC_GPIO_PORT         GPIOC

#define MIFOC_NTC2_ADC_CH                ADC_CHANNEL_9
#define MIFOC_NTC2_ADC_ORDER             1
#define MIFOC_NTC2_ADC_PIN               GPIO_PIN_0
#define MIFOC_NTC2_ADC_GPIO_PORT         GPIOB


#define WDG_HIGH_THRESHOLD               3500
#define WDG_LOW_THRESHOLD                100

extern uint16_t adc_value[4];

void adc_config(void);
void User_SofeTrig_RegularGroupConver(void);


#ifdef __cplusplus
}
#endif

#endif



