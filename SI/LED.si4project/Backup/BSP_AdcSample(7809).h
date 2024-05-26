#ifndef BSP_ADC_SAMPLE_H
#define BSP_ADC_SAMPLE_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "gd32f30x.h"

/* ADC for Motor Temp and BUS_VOL */

#define ADC_SAMP_CH_NUM                 3

#define MI_FOC_ADC                      ADC0
#define MI_FOC_ADC_CLK                  RCU_ADC0

#define MIFOC_VOL_ADC_CH                 ADC_CHANNEL_13
#define MIFOC_VOL_ADC_ORDER              0
#define MIFOC_VOL_ADC_PIN                GPIO_PIN_3
#define MIFOC_VOL_ADC_GPIO_PORT          GPIOC

#define MIFOC_NTC1_ADC_CH                ADC_CHANNEL_10
#define MIFOC_NTC1_ADC_ORDER             1
#define MIFOC_NTC1_ADC_PIN               GPIO_PIN_2
#define MIFOC_NTC1_ADC_GPIO_PORT         GPIOC

#define MIFOC_NTC2_ADC_CH                ADC_CHANNEL_8
#define MIFOC_NTC2_ADC_ORDER             2
#define MIFOC_NTC2_ADC_PIN               GPIO_PIN_0
#define MIFOC_NTC2_ADC_GPIO_PORT         GPIOB


void adc_config(void);


#ifdef __cplusplus
}
#endif

#endif



