/*!
    \file    gd32f307c_eval.h
    \brief   definitions for GD32F307C_EVAL's leds, keys and COM ports hardware resources

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x 
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef GD32F307C_EVAL_H
#define GD32F307C_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "gd32f30x.h"
#include "systick.h"

/* COM_Usart */
#define UART_DMA_ENABLE 1
#define CH_COUNT 10



/* configure COM port */
void Usart_com_init(uint32_t com);
void Usart_ComDataInit(void);
void UsartSendByte(uint32_t com, uint8_t Data);
void UsartSendStr(uint32_t com, char *Str);
void UsartSendUint16(uint32_t com, uint16_t Data);
void UsartSendFloat(uint32_t com, float Data);
void Usart_PC_PrintDeal(uint32_t com, uint8_t numofch);
uint8_t GetStringLength(const char* string);




typedef union uint16_data
{
	uint16_t u_data;
	uint8_t u_data_byte[2];
}uint16_frameDatatype;


typedef union float_data
{
	float f_data;
	uint8_t f_data_byte[4];
}float_frameDatatype;

typedef struct USART_FRAME
{
	float_frameDatatype* ch_data;
	uint8_t ch_num;
	uint8_t* check_tail;
	uint8_t check_length;
}Usart_Frame_DateType;

extern float_frameDatatype Usart_CH_Frame[CH_COUNT];

#ifdef __cplusplus
}
#endif

#endif /* GD32F307C_EVAL_H */
