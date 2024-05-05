#ifndef BSP_DRV8323RS_H
#define BSP_DRV8323RS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"

//PIN Config
#define GPIO_PORT_ENABLE      GPIOA
#define GPIO_PIN_ENABLE       GPIO_PIN_11

#define GPIO_PORT_FAULT       GPIOA
#define GPIO_PIN_FAULT        GPIO_PIN_12

#define GPIO_PORT_CS          GPIOB
#define GPIO_PIN_CS           GPIO_PIN_12

#define GPIO_PORT_SCK         GPIOB
#define GPIO_PIN_SCK          GPIO_PIN_13

#define GPIO_PORT_MOSI        GPIOB
#define GPIO_PIN_MOSI         GPIO_PIN_15

#define GPIO_PORT_MISO        GPIOB
#define GPIO_PIN_MISO        GPIO_PIN_14



//ENABLE & CS片选状态
#define ENABLE_DRV8323        gpio_bit_set(GPIOA,GPIO_PIN_11);
#define DISABLE_DRV8323       gpio_bit_reset(GPIOA,GPIO_PIN_11);

#define SET_DRV8323_NSS_HIGH  gpio_bit_set(GPIOB,GPIO_PIN_12);
#define SET_DRV8323_NSS_LOW   gpio_bit_reset(GPIOB,GPIO_PIN_12);

//读写指令宏定义
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

//故障标志位
#define FAULT_BIT(x)              ((uint16_t)((uint16_t)0x01U<<(x)))
#define VDS_OCP                   FAULT_BIT(9)

void spi1_config(void);
uint16_t GenCmd_drv(uint8_t Adress, uint8_t RW, uint16_t Data);
uint16_t  Drv8323_ReadData(uint8_t address);
uint16_t Drv8323_WriteCmd(uint8_t address,uint16_t cmd);



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




