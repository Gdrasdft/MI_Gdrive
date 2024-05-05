/*
 * File: MI_FOC.c
 *
 * Code generated for Simulink model 'MI_FOC'.
 *
 * Model version                  : 1.235
 * Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
 * C/C++ source code generated on : Fri Oct  6 18:37:36 2023
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "MI_FOC.h"
#include "rtwtypes.h"
#include "mw_cmsis.h"

/* Exported data definition */

/* Definition for custom storage class: Struct */
AntiPark_Vol_type AntiPark_Vol;
ClarkeCur_type ClarkeCur;
FOC_Input_VolCur_type FOC_Input_VolCur;
Park_Cur_type Park_Cur;
Rotor_Pos_type Rotor_Pos;
SVPWM_OutCmp_type SVPWM_OutCmp;

/* Block signals and states (default storage) */
DW rtDW;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;
static void AntiPark_Trans(real32_T rtu_Ud, real32_T rtu_Uq, real32_T
  rtu_SinTheta, real32_T rtu_CosTheta, real32_T *rty_Ualpha, real32_T *rty_Ubeta);
static void Clarke_Trans(real32_T rtu_CurA, real32_T rtu_CurB, real32_T rtu_CurC,
  real32_T *rty_Ialpha, real32_T *rty_Ibeta);
static void Park_Trans(real32_T rtu_Ialpha, real32_T rtu_Ibeta, real32_T
  rtu_SinTheta, real32_T rtu_CosTheta, real32_T *rty_Id, real32_T *rty_Iq);
static void SVPWM(real32_T rtu_Valpha, real32_T rtu_Vbeta, real32_T rtu_Udc,
                  real32_T *rty_Tcmp1, real32_T *rty_Tcmp2, real32_T *rty_Tcmp3,
                  real32_T *rty_sector);
static void ThreePhaseCurGen(real32_T *rty_CurA, real32_T *rty_CurB, real32_T
  *rty_CurC, real32_T *rty_Theta, DW_ThreePhaseCurGen *localDW);

/* Output and update for atomic system: '<S1>/Anti_Park' */
static void AntiPark_Trans(real32_T rtu_Ud, real32_T rtu_Uq, real32_T
  rtu_SinTheta, real32_T rtu_CosTheta, real32_T *rty_Ualpha, real32_T *rty_Ubeta)
{
  /* Sum: '<S2>/Add' incorporates:
   *  Product: '<S2>/Product'
   *  Product: '<S2>/Product1'
   */
  *rty_Ualpha = rtu_Ud * rtu_CosTheta - rtu_Uq * rtu_SinTheta;

  /* Sum: '<S2>/Add1' incorporates:
   *  Product: '<S2>/Product2'
   *  Product: '<S2>/Product3'
   */
  *rty_Ubeta = rtu_Ud * rtu_SinTheta + rtu_Uq * rtu_CosTheta;
}

/* Output and update for atomic system: '<S1>/Clarke' */
static void Clarke_Trans(real32_T rtu_CurA, real32_T rtu_CurB, real32_T rtu_CurC,
  real32_T *rty_Ialpha, real32_T *rty_Ibeta)
{
  /* Sum: '<S3>/Add' incorporates:
   *  Gain: '<S3>/Gain_IA'
   *  Gain: '<S3>/Gain_IB'
   *  Gain: '<S3>/Gain_IC'
   */
  *rty_Ialpha = (0.666666687F * rtu_CurA + -0.333333343F * rtu_CurB) +
    -0.333333343F * rtu_CurC;

  /* Sum: '<S3>/Add1' incorporates:
   *  Gain: '<S3>/Gain_IB1'
   *  Gain: '<S3>/Gain_IC1'
   */
  *rty_Ibeta = 0.577350259F * rtu_CurB + -0.577350259F * rtu_CurC;
}

/* Output and update for atomic system: '<S1>/Park' */
static void Park_Trans(real32_T rtu_Ialpha, real32_T rtu_Ibeta, real32_T
  rtu_SinTheta, real32_T rtu_CosTheta, real32_T *rty_Id, real32_T *rty_Iq)
{
  /* Sum: '<S4>/Add' incorporates:
   *  Product: '<S4>/Product'
   *  Product: '<S4>/Product1'
   */
  *rty_Id = rtu_Ialpha * rtu_CosTheta + rtu_Ibeta * rtu_SinTheta;

  /* Sum: '<S4>/Add1' incorporates:
   *  Product: '<S4>/Product2'
   *  Product: '<S4>/Product3'
   */
  *rty_Iq = rtu_Ibeta * rtu_CosTheta - rtu_Ialpha * rtu_SinTheta;
}

/* Output and update for atomic system: '<S1>/SVPWM' */
static void SVPWM(real32_T rtu_Valpha, real32_T rtu_Vbeta, real32_T rtu_Udc,
                  real32_T *rty_Tcmp1, real32_T *rty_Tcmp2, real32_T *rty_Tcmp3,
                  real32_T *rty_sector)
{
  int32_T sector;
  real32_T T1;
  real32_T T2;
  real32_T ta;
  sector = 0;
  *rty_Tcmp1 = 0.0F;
  *rty_Tcmp2 = 0.0F;
  *rty_Tcmp3 = 0.0F;
  if (rtu_Vbeta > 0.0F) {
    sector = 1;
  }

  if ((1.73205078F * rtu_Valpha - rtu_Vbeta) / 2.0F > 0.0F) {
    sector += 2;
  }

  if ((-1.73205078F * rtu_Valpha - rtu_Vbeta) / 2.0F > 0.0F) {
    sector += 4;
  }

  switch (sector) {
   case 1:
    T1 = (-1.5F * rtu_Valpha + 0.866025388F * rtu_Vbeta) * ((real32_T)((uint16_T)
      Time_Base) / rtu_Udc);
    T2 = (1.5F * rtu_Valpha + 0.866025388F * rtu_Vbeta) * ((real32_T)((uint16_T)
      Time_Base) / rtu_Udc);
    break;

   case 2:
    T1 = (1.5F * rtu_Valpha + 0.866025388F * rtu_Vbeta) * ((real32_T)((uint16_T)
      Time_Base) / rtu_Udc);
    T2 = -((real32_T)((uint16_T)Time_Base) / rtu_Udc * 1.73205078F * rtu_Vbeta);
    break;

   case 3:
    T1 = -((-1.5F * rtu_Valpha + 0.866025388F * rtu_Vbeta) * ((real32_T)
            ((uint16_T)Time_Base) / rtu_Udc));
    T2 = (real32_T)((uint16_T)Time_Base) / rtu_Udc * 1.73205078F * rtu_Vbeta;
    break;

   case 4:
    T1 = -((real32_T)((uint16_T)Time_Base) / rtu_Udc * 1.73205078F * rtu_Vbeta);
    T2 = (-1.5F * rtu_Valpha + 0.866025388F * rtu_Vbeta) * ((real32_T)((uint16_T)
      Time_Base) / rtu_Udc);
    break;

   case 5:
    T1 = (real32_T)((uint16_T)Time_Base) / rtu_Udc * 1.73205078F * rtu_Vbeta;
    T2 = -((1.5F * rtu_Valpha + 0.866025388F * rtu_Vbeta) * ((real32_T)
            ((uint16_T)Time_Base) / rtu_Udc));
    break;

   default:
    T1 = -((1.5F * rtu_Valpha + 0.866025388F * rtu_Vbeta) * ((real32_T)
            ((uint16_T)Time_Base) / rtu_Udc));
    T2 = -((-1.5F * rtu_Valpha + 0.866025388F * rtu_Vbeta) * ((real32_T)
            ((uint16_T)Time_Base) / rtu_Udc));
    break;
  }

  ta = T1 + T2;
  if (ta > ((uint16_T)Time_Base)) {
    T1 /= ta;
    T2 /= T1 + T2;
  }

  ta = ((real32_T)((uint16_T)Time_Base) - (T1 + T2)) / 4.0F;
  T1 = T1 / 2.0F + ta;
  switch (sector) {
   case 1:
    *rty_Tcmp1 = T1;
    *rty_Tcmp2 = ta;
    *rty_Tcmp3 = T2 / 2.0F + T1;
    break;

   case 2:
    *rty_Tcmp1 = ta;
    *rty_Tcmp2 = T2 / 2.0F + T1;
    *rty_Tcmp3 = T1;
    break;

   case 3:
    *rty_Tcmp1 = ta;
    *rty_Tcmp2 = T1;
    *rty_Tcmp3 = T2 / 2.0F + T1;
    break;

   case 4:
    *rty_Tcmp1 = T2 / 2.0F + T1;
    *rty_Tcmp2 = T1;
    *rty_Tcmp3 = ta;
    break;

   case 5:
    *rty_Tcmp1 = T2 / 2.0F + T1;
    *rty_Tcmp2 = ta;
    *rty_Tcmp3 = T1;
    break;

   case 6:
    *rty_Tcmp1 = T1;
    *rty_Tcmp2 = T2 / 2.0F + T1;
    *rty_Tcmp3 = ta;
    break;
  }

  *rty_sector = (real32_T)sector;
}

/* Output and update for atomic system: '<S1>/ThreePhase' */
static void ThreePhaseCurGen(real32_T *rty_CurA, real32_T *rty_CurB, real32_T
  *rty_CurC, real32_T *rty_Theta, DW_ThreePhaseCurGen *localDW)
{
  real32_T Angle_out;
  real32_T Angle_out1;
  real32_T Angle_out2;

  /* MATLAB Function: '<S7>/Angle_Generate' */
  if (localDW->Angle >= 360.0F) {
    localDW->Angle = 0.0F;
  } else {
    localDW->Angle += 360.0F / (real32_T)((uint16_T)Time_Base);
  }

  Angle_out = -localDW->Angle / 180.0F * PI;
  Angle_out1 = -(localDW->Angle + 120.0F) / 180.0F * PI;
  Angle_out2 = -(localDW->Angle - 120.0F) / 180.0F * PI;
  if (Angle_out >= 2.0F * PI) {
    Angle_out -= 2.0F * PI;
  } else if (Angle_out <= -2.0F * PI) {
    Angle_out += 2.0F * PI;
  }

  if (Angle_out1 >= 2.0F * PI) {
    Angle_out1 -= 2.0F * PI;
  } else if (Angle_out1 <= -2.0F * PI) {
    Angle_out1 += 2.0F * PI;
  }

  if (Angle_out2 >= 2.0F * PI) {
    Angle_out2 = Angle_out1 - 2.0F * PI;
  } else if (Angle_out2 <= -2.0F * PI) {
    Angle_out2 = 2.0F * PI + Angle_out1;
  }

  /* Trigonometry: '<S6>/Sin1' incorporates:
   *  MATLAB Function: '<S7>/Angle_Generate'
   */
  *rty_CurB = arm_sin_f32(Angle_out1);

  /* Trigonometry: '<S6>/Sin2' incorporates:
   *  MATLAB Function: '<S7>/Angle_Generate'
   */
  *rty_CurC = arm_sin_f32(Angle_out2);

  /* DataTypeConversion: '<S6>/Data Type Conversion' incorporates:
   *  MATLAB Function: '<S7>/Angle_Generate'
   */
  *rty_Theta = Angle_out;

  /* Trigonometry: '<S6>/Sin' */
  *rty_CurA = arm_sin_f32(*rty_Theta);
}

/* Model step function */
void MI_FOC_step(void)
{
  /* Outputs for Atomic SubSystem: '<Root>/MI_FOC' */
  /* Outputs for Atomic SubSystem: '<S1>/ThreePhase' */
  /* SignalConversion generated from: '<S1>/ThreePhase' */
  ThreePhaseCurGen(&(FOC_Input_VolCur.Ia), &(FOC_Input_VolCur.Ib),
                   &(FOC_Input_VolCur.Ic), &(Rotor_Pos.Theta), &rtDW.ThreePhase);

  /* End of Outputs for SubSystem: '<S1>/ThreePhase' */

  /* Trigonometry: '<S1>/Sin' */
  Rotor_Pos.SinTheta = arm_sin_f32(Rotor_Pos.Theta);

  /* Trigonometry: '<S1>/Cos' */
  Rotor_Pos.CosTheta = arm_cos_f32(Rotor_Pos.Theta);

  /* Outputs for Atomic SubSystem: '<S1>/Clarke' */
  /* SignalConversion generated from: '<S1>/Clarke' */
  Clarke_Trans(FOC_Input_VolCur.Ia, FOC_Input_VolCur.Ib, FOC_Input_VolCur.Ic,
               &(ClarkeCur.Ialpha), &(ClarkeCur.Ibeta));

  /* End of Outputs for SubSystem: '<S1>/Clarke' */

  /* Outputs for Atomic SubSystem: '<S1>/Park' */
  /* SignalConversion generated from: '<S1>/Park' */
  Park_Trans(ClarkeCur.Ialpha, ClarkeCur.Ibeta, Rotor_Pos.SinTheta,
             Rotor_Pos.CosTheta, &(Park_Cur.Id), &(Park_Cur.Iq));

  /* End of Outputs for SubSystem: '<S1>/Park' */

  /* Outputs for Atomic SubSystem: '<S1>/Anti_Park' */
  /* SignalConversion generated from: '<S1>/Anti_Park' */
  AntiPark_Trans(Park_Cur.Id, Park_Cur.Iq, Rotor_Pos.SinTheta,
                 Rotor_Pos.CosTheta, &(AntiPark_Vol.Ualpha),
                 &(AntiPark_Vol.Ubeta));

  /* End of Outputs for SubSystem: '<S1>/Anti_Park' */

  /* SignalConversion generated from: '<S1>/SVPWM' incorporates:
   *  MATLAB Function: '<S1>/SVPWM'
   */
  SVPWM(AntiPark_Vol.Ualpha, AntiPark_Vol.Ubeta, FOC_Input_VolCur.Udc,
        &(SVPWM_OutCmp.Tcmp1), &(SVPWM_OutCmp.Tcmp2), &(SVPWM_OutCmp.Tcmp3),
        &(SVPWM_OutCmp.sector));

  /* End of Outputs for SubSystem: '<Root>/MI_FOC' */
}

/* Model initialize function */
void MI_FOC_initialize(void)
{
  /* SystemInitialize for Atomic SubSystem: '<Root>/MI_FOC' */
  /* Start for Constant: '<S1>/Constant' */
  FOC_Input_VolCur.Udc = 24.0F;

  /* End of SystemInitialize for SubSystem: '<Root>/MI_FOC' */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
