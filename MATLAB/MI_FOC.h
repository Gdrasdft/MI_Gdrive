/*
 * File: MI_FOC.h
 *
 * Code generated for Simulink model 'MI_FOC'.
 *
 * Model version                  : 1.240
 * Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
 * C/C++ source code generated on : Mon Jun  3 20:20:08 2024
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

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T Ia;                         /* '<Root>/Ia' */
  real32_T Ib;                         /* '<Root>/Ib' */
  real32_T Ic;                         /* '<Root>/Ic' */
  real32_T Theta;                      /* '<Root>/Theta' */
  real32_T Iq_set;                     /* '<Root>/Iq_set' */
  real32_T Id_set;                     /* '<Root>/Id_set' */
} ExtU;

/* Type definition for custom storage class: Struct */
typedef struct AntiPark_Vol_tag {
  real32_T Ualpha;                     /* '<S1>/Anti_Park' */
  real32_T Ubeta;                      /* '<S1>/Anti_Park' */
} AntiPark_Vol_type;

typedef struct ClarkeCur_tag {
  real32_T Ialpha;                     /* '<S1>/Clarke' */
  real32_T Ibeta;                      /* '<S1>/Clarke' */
} ClarkeCur_type;

typedef struct FOC_Input_VolCur_tag {
  real32_T Udc;                        /* '<S1>/Constant' */
} FOC_Input_VolCur_type;

typedef struct Park_Cur_tag {
  real32_T Id;                         /* '<S1>/Park' */
  real32_T Iq;                         /* '<S1>/Park' */
} Park_Cur_type;

typedef struct Rotor_Pos_tag {
  real32_T SinTheta;                   /* '<S1>/Sin' */
  real32_T CosTheta;                   /* '<S1>/Cos' */
} Rotor_Pos_type;

typedef struct SVPWM_OutCmp_tag {
  real32_T Tcmp1;                      /* '<S1>/SVPWM' */
  real32_T Tcmp2;                      /* '<S1>/SVPWM' */
  real32_T Tcmp3;                      /* '<S1>/SVPWM' */
  real32_T sector;                     /* '<S1>/SVPWM' */
} SVPWM_OutCmp_type;

/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/*
 * Exported Global Parameters
 *
 * Note: Exported global parameters are tunable parameters with an exported
 * global storage class designation.  Code generation will declare the memory for
 * these parameters and exports their symbols.
 *
 */
extern uint16_T Time_Base;             /* Variable: Time_Base
                                        * Referenced by: '<S1>/SVPWM'
                                        */

/*
 * Exported States
 *
 * Note: Exported states are block states with an exported global
 * storage class designation.  Code generation will declare the memory for these
 * states and exports their symbols.
 *
 */
extern real32_T Ud_Set;                /* '<S1>/Data Store Memory' */
extern real32_T Uq_Set;                /* '<S1>/Data Store Memory1' */

/* Model entry point functions */
extern void MI_FOC_initialize(void);
extern void MI_FOC_step(void);

/* Exported data declaration */

/* Declaration for custom storage class: Struct */
extern AntiPark_Vol_type AntiPark_Vol;
extern ClarkeCur_type ClarkeCur;
extern FOC_Input_VolCur_type FOC_Input_VolCur;
extern Park_Cur_type Park_Cur;
extern Rotor_Pos_type Rotor_Pos;
extern SVPWM_OutCmp_type SVPWM_OutCmp;

/* Real-time Model object */
extern RT_MODEL *const rtM;

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
 * '<S2>'   : 'FOC_Simulink_Modle/MI_FOC/Anti_Park'
 * '<S3>'   : 'FOC_Simulink_Modle/MI_FOC/Clarke'
 * '<S4>'   : 'FOC_Simulink_Modle/MI_FOC/Park'
 * '<S5>'   : 'FOC_Simulink_Modle/MI_FOC/SVPWM'
 */
#endif                                 /* RTW_HEADER_MI_FOC_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
