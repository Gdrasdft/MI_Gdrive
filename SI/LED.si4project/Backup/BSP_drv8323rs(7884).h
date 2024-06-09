#ifndef BSP_DRV8323RS_H
#define BSP_DRV8323RS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"
#include <stdio.h>

//PIN Config
#define GPIO_PORT_ENABLE      GPIOA
#define GPIO_PIN_ENABLE       GPIO_PIN_11

#define GPIO_PORT_FAULT             GPIOA
#define GPIO_CLK_FAULT             RCU_GPIOA
#define GPIO_PIN_FAULT              GPIO_PIN_12
#define FAULT_EXTI_LINE             EXTI_12
#define FAULT_EXTI_PORT_SOURCE      GPIO_PORT_SOURCE_GPIOA
#define FAULT_EXTI_PIN_SOURCE       GPIO_PIN_SOURCE_12
#define FAULT_EXTI_IRQn             EXTI10_15_IRQn


#define GPIO_PORT_CS          GPIOB
#define GPIO_PIN_CS           GPIO_PIN_12

#define GPIO_PORT_SCK         GPIOB
#define GPIO_PIN_SCK          GPIO_PIN_13

#define GPIO_PORT_MOSI        GPIOB
#define GPIO_PIN_MOSI         GPIO_PIN_15

#define GPIO_PORT_MISO        GPIOB
#define GPIO_PIN_MISO         GPIO_PIN_14



//ENABLE & CS片选状态
#define ENABLE_DRV8323        gpio_bit_set(GPIOA,GPIO_PIN_11);
#define DISABLE_DRV8323       gpio_bit_reset(GPIOA,GPIO_PIN_11);

#define SET_DRV8323_NSS_HIGH  gpio_bit_set(GPIOB,GPIO_PIN_12);
#define SET_DRV8323_NSS_LOW   gpio_bit_reset(GPIOB,GPIO_PIN_12);

//故障标志位
enum DRV_ERRCODE
{
	VGS_LC = 0, 	                       //C相下桥MOS驱动故障
	VGS_HC = 1,					   	           //C相上桥MOS驱动故障
	VGS_LB = 2,					   	           //B相下桥MOS驱动故障
	VGS_HB = 3,					   	           //B相上桥MOS驱动故障
	VGS_LA = 4,					   	           //A相下桥MOS驱动故障
	VGS_HA = 5,					   	           //A相上桥MOS驱动故障
	CPUV   = 6,					  	           //电荷泵输出欠压
	OTW    = 7,					   	           //过温预警
	SC_OC  = 8,					   	           //A相输出过流
	SB_OC  = 9,					   	           //A相输出过流
	SA_OC  = 10,					  	           //A相输出过流
	VDS_LC = 11,					               //C相下桥臂VDS过流
	VDS_HC = 12,					               //C相上桥臂VDS过流
	VDS_LB = 13,					               //B相下桥臂VDS过流
	VDS_HB = 14,					               //B相上桥臂VDS过流
	VDS_LA = 15,					               //A相下桥臂VDS过流
	VDS_HA = 16,				                   //A相上桥臂VDS过流
	OTSD   = 17,					               //过温关断故障
	UVLO   = 18,					               //驱动欠压故障
	GDF    = 19,					               //栅极驱动故障
	VDS_OCP= 20, 				               //VDS过流故障
	GEN_FAULT = 21,				               //故障总标志位
	ERRMAX
};

// 寄存器地址
#define Fault_Status1     0x00
#define Fault_Status2     0x01
#define Driver_Control    0x02
#define Gate_Drive_HS     0x03
#define Gate_Drive_LS     0x04
#define OCP_Control       0x05
#define CSA_Control       0x06

//驱动控制寄存器 0x02
typedef struct DRI_CTL_REG
{
	uint16_t CLR_FLT          :1;
	uint16_t ONEPWM_BRAKE     :1;	
	uint16_t COAST            :1;
	uint16_t ONEPWM_DIR       :1;
	uint16_t ONEPWM_COM       :1;
	uint16_t PWM_MODE         :2;
	uint16_t OTW_REP          :1;
	uint16_t DIS_GDF          :1;
	uint16_t DIS_CPUV         :1;
	uint16_t RSVD             :6;	
}Dri_Ctl_Reg;
typedef union DRI_CTL
{
	Dri_Ctl_Reg reg;
	uint16_t cmd;
} Dri_Ctl;

//高桥栅极控制寄存器 0x03
typedef struct GATE_DRIHS_REG
{
	uint16_t IDRIVEN_HS       :4;
	uint16_t IDRIVEP_HS       :4;
	uint16_t LOCK             :3;
	uint16_t RSVD             :5;
}Gate_DriHS_Reg;
typedef union GATE_HS
{
	Gate_DriHS_Reg reg;
	uint16_t cmd;
} Gate_HS;

//低桥栅极控制寄存器 0x04
typedef struct GATE_DRILS_REG
{
	uint16_t IDRIVEN_LS       :4;
	uint16_t IDRIVEP_LS 	  :4;
	uint16_t TDRIVE           :2;
	uint16_t CBC              :1;
	uint16_t RSVD             :5;	
}Gate_DriLS_Reg;
typedef union GATE_LS
{
	Gate_DriLS_Reg reg;
	uint16_t cmd;
} Gate_LS;

//过流保护寄存器 0x05
typedef struct OCP_CTL_REG
{
	uint16_t VDS_LVL          :4;
	uint16_t OCP_DEG          :2;
	uint16_t OCP_MODE         :2;
	uint16_t DEADTIME         :2;
	uint16_t TRETRY           :1;
	uint16_t RSVD             :5;
}Ocp_Ctl_Reg;
typedef union OCP_CTL
{
	Ocp_Ctl_Reg reg;
	uint16_t cmd;
} Ocp_Ctl;

//电流检测寄存器 0x06
typedef struct CSA_CTL_REG
{

	uint16_t SEN_LVL          :2;
	uint16_t CSA_CAL_C        :1;
	uint16_t CSA_CAL_B        :1;
	uint16_t CSA_CAL_A        :1;
	uint16_t DIS_SEN          :1;
	uint16_t CSA_GAIN         :2;
	uint16_t LS_REF           :1;
	uint16_t VREF_DIV         :1;
	uint16_t CSA_FET          :1;
	uint16_t RSVD             :5;
}Csa_Ctl_Reg;
typedef union CSA_CTL
{
	Csa_Ctl_Reg reg;
	uint16_t cmd;
} Csa_Ctl;

/* ==================================================================================== */
//读写指令宏定义
#define READ_DATA 1
#define WRITE_CMD 0

//命令帧解析结构体
typedef struct CMD_BIT_STR_DRV
{
	uint16_t DATA :11;
	uint16_t ADDR :4;
	uint16_t RW   :1;
}Cmd_Bit_Str_Drv;

typedef union CMD_FRAME_DRV
{
	Cmd_Bit_Str_Drv cmd_bit_drv;
	uint16_t cmd_send_drv;
} Cmd_Frame_Drv;

//数据帧解析结构体
typedef struct DATA_BIT_STR_DRV
{
	uint16_t DATA       :11;
	uint16_t Reserved   :5;
}Data_Bit_Str_Drv;

typedef union DATA_FRAME_DRV
{
	Data_Bit_Str_Drv data_bit_drv;
	uint16_t data_receive_drv;
}Data_Frame_Drv;
/* ==================================================================================== */


void spi1_config(void);
uint16_t DRV8323_Init_Device(void);
void DRV8323_Init_ResultPrint(void);
void Report_Drv8323_FaultInfo(void);
void DRV8323_DeviceReset(void);
void DRV8323_FaultReset(void);

uint16_t  Drv8323_ReadData(uint8_t address);
uint16_t Drv8323_WriteCmd(uint8_t address,uint16_t cmd);

extern uint32_t Drv8323_FaultInfo;
extern uint16_t drv8323rs_data[4];
extern uint8_t ErrCode[ERRMAX];
extern Dri_Ctl DRI_CTL_DATA;
extern Gate_HS GATE_HS_DATA;
extern Gate_LS GATE_LS_DATA;
extern Ocp_Ctl OCP_CTL_DATA;
extern Csa_Ctl OSA_CTL_DATA;


#ifdef __cplusplus
}
#endif

#endif 




