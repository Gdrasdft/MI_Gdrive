/*
 * File: MI_FOC.h
 *
 * Code generated for Simulink model 'MI_FOC'.
 *
 * Model version                  : 1.144
 * Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
 * C/C++ source code generated on : Fri Oct  6 12:50:03 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_MI_FOC_h_
#define RTW_HEADER_MI_FOC_h_
#ifndef MI_FOC_COMMON_INCLUDES_
#define MI_FOC_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* MI_FOC_COMMON_INCLUDES_ */

/* Model Code Variants */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#define MI_FOC_M                       (rtM)

/* Exported data define */

/* Definition for custom storage class: Define */
//#define PI                             3.14159274F               /* Referenced by: '<S4>/Angle_Generate' */
#define Time_Base                      1000U                    /* Referenced by: '<S4>/Angle_Generate' */

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<S1>/ThreePhase' */
typedef struct {
  real32_T Angle;                      /* '<S4>/Angle_Generate' */
} DW_ThreePhaseCurGen;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  DW_ThreePhaseCurGen ThreePhase;      /* '<S1>/ThreePhase' */
} DW;

/* Type definition for custom storage class: Struct */
typedef struct ClarkeCur_tag {
  real32_T Ialpha;                     /* '<S1>/Clarke' */
  real32_T Ibeta;                      /* '<S1>/Clarke' */
} ClarkeCur_type;

typedef struct ThreePhaseCur_tag {
  real32_T Ia;                         /* '<S1>/ThreePhase' */
  real32_T Ib;                         /* '<S1>/ThreePhase' */
  real32_T Ic;                         /* '<S1>/ThreePhase' */
} ThreePhaseCur_type;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* Block signals and states (default storage) */
extern DW rtDW;

/* Model entry point functions */
extern void MI_FOC_initialize(void);
extern void MI_FOC_step(void);

/* Exported data declaration */

/* Declaration for custom storage class: Struct */
extern ClarkeCur_type ClarkeCur;
extern ThreePhaseCur_type ThreePhaseCur;

/* Real-time Model object */
extern RT_MODEL *const rtM;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Scope' : Unused code path elimination
 * Block '<S3>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S3>/Data Type Conversion1' : Eliminate redundant data type conversion
 * Block '<S3>/Data Type Conversion2' : Eliminate redundant data type conversion
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('FOC_Simulink_Modle/MI_FOC')    - opens subsystem FOC_Simulink_Modle/MI_FOC
 * hilite_system('FOC_Simulink_Modle/MI_FOC/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'FOC_Simulink_Modle'
 * '<S1>'   : 'FOC_Simulink_Modle/MI_FOC'
 * '<S2>'   : 'FOC_Simulink_Modle/MI_FOC/Clarke'
 * '<S3>'   : 'FOC_Simulink_Modle/MI_FOC/ThreePhase'
 * '<S4>'   : 'FOC_Simulink_Modle/MI_FOC/ThreePhase/Freq_Gener'
 * '<S5>'   : 'FOC_Simulink_Modle/MI_FOC/ThreePhase/Freq_Gener/Angle_Generate'
 */
#endif                                 /* RTW_HEADER_MI_FOC_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
