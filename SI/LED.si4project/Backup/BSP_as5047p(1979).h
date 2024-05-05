#ifndef BSP_AS5047P_H
#define BSP_AS5047P_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"


//CS片选状态

#define SET_SPI2_NSS_HIGH gpio_bit_set(GPIOA,GPIO_PIN_11);
#define SET_SPI2_NSS_LOW  gpio_bit_reset(GPIOA,GPIO_PIN_11);

#define READ_DATA 1
#define WRITE_CMD 0

#define EVEN_CHECK_LEN 15

// 寄存器地址
#define AS5047_NOP     0x0000
#define AS5047_ERRFL   0x0001
#define AS5047_PROG    0x0003
#define AS5047_DIAAGC  0x33FC
#define AS5047_MAG     0x33FD
#define AS5047_ANGLEUNC 0x3FFE
#define AS5047_ANGLECOM 0x3FFF

void spi_config(void);
uint16_t GenCmd2Read(uint16_t Adress);
uint16_t GenCmd2Write(uint16_t Command);
uint16_t  AS5047_GetAngle(void);




//命令帧解析结构体
typedef struct CMD_BIT_STR
{
	uint16_t ADDR :14;
	uint16_t RW   :1;
	uint16_t PARC :1;
}Cmd_Bit_Str;

typedef union CMD_FRAME
{
	Cmd_Bit_Str cmd_bit;
	uint16_t cmd_send;
} Cmd_Frame;

//数据帧解析结构体
typedef struct DATA_BIT_STR
{
	uint16_t DATA :14;
	uint16_t EF   :1;
	uint16_t PARD :1;
}Data_Bit_Str;

typedef union DATA_FRAME
{
	Data_Bit_Str data_bit;
	uint16_t data_receive;
}Data_Frame;




#ifdef __cplusplus
}
#endif

#endif 




