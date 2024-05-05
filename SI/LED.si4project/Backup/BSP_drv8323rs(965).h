#ifndef BSP_DRV8323RS_H
#define BSP_DRV8323RS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"

//CS片选状态

#define SET_DRV8323_NSS_HIGH  gpio_bit_set(GPIOB,GPIO_PIN_12);
#define SET_DRV8323_NSS_LOW   gpio_bit_reset(GPIOB,GPIO_PIN_12);

#define READ_DATA 1
#define WRITE_CMD 0

// 寄存器地址
#define Fault_Status1     0x00
#define Fault_Status2     0x01
#define Driver_Control    0x02
#define Gate_Drive_HS     0x03
#define Gate_Drive_LS     0x04
#define OCP_Control       0x05
#define CSA_Control       0x06

void spi1_config(void);
uint16_t GenCmd_drv(uint16_t Adress, uint8_t RW);




//命令帧解析结构体
typedef struct CMD_BIT_STR
{
	uint16_t DATA :11;
	uint16_t ADDR :4;
	uint16_t RW   :1;
}Cmd_Bit_Str_Drv;

typedef union CMD_FRAME
{
	Cmd_Bit_Str_Drv cmd_bit;
	uint16_t cmd_send;
} Cmd_Frame_Drv;

//数据帧解析结构体
typedef struct DATA_BIT_STR
{
	uint16_t DATA :14;
	uint16_t EF   :1;
	uint16_t PARD :1;
}Data_Bit_Str_Drv;

typedef union DATA_FRAME
{
	Data_Bit_Str_Drv data_bit;
	uint16_t data_receive;
}Data_Frame_Drv;





#ifdef __cplusplus
}
#endif

#endif 




