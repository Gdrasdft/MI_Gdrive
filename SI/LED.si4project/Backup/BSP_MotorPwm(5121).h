#ifndef BSP_MOTOR_PWM_H
#define BSP_MOTOR_PWM_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "gd32f30x.h"

#define CNT_DIR_UP   0
#define CNT_DIR_DOWN 1

#define fTIMER_CK    120000000
#define TIMER_CNT_NS 8
#define INIT_FREQ    10000


#define ADC_SAMPLE_DELAY_NS  1000
#define ADC_SAMPLE_DELAY_CNT (ADC_SAMPLE_DELAY_NS/TIMER_CNT_NS)



extern timer_oc_parameter_struct timer_ocintpara;
extern timer_parameter_struct timer_initpara;
extern uint8_t TimerCouner_Dir;
extern uint32_t UpCountNum;
extern uint32_t DowmCountNum;

void timer_config(void);



#ifdef __cplusplus
}
#endif

#endif



