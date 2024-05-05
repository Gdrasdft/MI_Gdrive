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
uint16_t GenCmd_drv(uint8_t Adress, uint8_t RW, uint16_t Data);
uint16_t  Drv8323_ReadData(void);





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





#ifdef __cplusplus
}
#endif

#endif 




