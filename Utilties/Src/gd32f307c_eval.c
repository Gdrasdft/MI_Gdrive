/*!
    \file    gd32f307c_eval.c
    \brief   firmware functions to manage leds, keys, COM ports

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

#include <gd32f30x.h>
#include "gd32f307c_eval.h"

/* UART_DMA_channel selection */
dma_channel_enum DMA_CH;
/* UART_DMA_Check_Tail --- 通讯校验 */
uint8_t Usart_CheckTail[4]={0x00, 0x00, 0x80, 0x7f};
/* UART_DMA_FrameData  --- 通讯报文 */
Usart_Frame_DateType Usart_Frame_DataInfo;
/* UART_DMA_OriginData --- 通讯存储 */
float_frameDatatype Usart_CH_Frame[CH_COUNT];

void Usart_ComDataInit(void)
{
	Usart_Frame_DataInfo.ch_data = Usart_CH_Frame;
	Usart_Frame_DataInfo.ch_num = CH_COUNT;
	Usart_Frame_DataInfo.check_tail = Usart_CheckTail;
	Usart_Frame_DataInfo.check_length = sizeof(Usart_CheckTail)/sizeof(Usart_CheckTail[0]);
}

#if UART_DMA_ENABLE
void usart_TX_dma_init(uint32_t com)
{
	/* UART_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;
	if(USART0 == com)
	{
		DMA_CH = DMA_CH3;
	}
	if(USART1 == com)
	{
		DMA_CH = DMA_CH6;
	}
	if(USART2 == com)
	{
		DMA_CH = DMA_CH1;
	}
	/* UART DMA_channel configuration */
    dma_deinit(DMA0, DMA_CH);
	  
    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr  = (uint32_t)(&USART_DATA(com));
    dma_data_parameter.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr  = (uint32_t)(Usart_CH_Frame);
    dma_data_parameter.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_MEMORY_WIDTH_8BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_8BIT;  
    dma_data_parameter.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_data_parameter.number       = 4;
    dma_data_parameter.priority     = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH, &dma_data_parameter);
	
	/* Disable DMA circle Transfer */
    dma_circulation_disable(DMA0, DMA_CH);

    /* enable DMA channel */
    dma_channel_disable(DMA0, DMA_CH);
}
#endif

/*!
    \brief      configure COM port
    \param[in]  com: COM on the board
      \arg        EVAL_COM0: COM0 on the board
      \arg        EVAL_COM1: COM1 on the board
    \param[out] none
    \retval     none
*/
void Usart_com_init(uint32_t com)
{
    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, 500000U);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
	
#if UART_DMA_ENABLE
	/* DMA configure */
	usart_flag_clear(com,USART_FLAG_TC);
	usart_dma_transmit_config(com,USART_DENT_ENABLE);
	usart_TX_dma_init(com);
#endif

    usart_enable(com);
}

void UsartSendByte(uint32_t com, uint8_t Data)
{
	usart_data_transmit(com, Data);
	while(RESET == usart_flag_get(com, USART_FLAG_TBE));
}

void UsartSendStr(uint32_t com, char *DataString)
{
	uint8_t cnt = 0;
	while(DataString[cnt] != '\0')
	{
		usart_data_transmit(com, DataString[cnt]);
		while(RESET == usart_flag_get(com, USART_FLAG_TBE));
		cnt++;
	}
	while(RESET == usart_flag_get(com, USART_FLAG_TBE));
}

void UsartSendUint16(uint32_t com, uint16_t Data)
{
	uint16_frameDatatype temp_data;
	temp_data.u_data = Data;
	for(uint8_t i=0; i<2; i++)
	{
		UsartSendByte(com, temp_data.u_data_byte[i]);
	}
}

void UsartSendFloat(uint32_t com, float Data)
{
	float_frameDatatype temp_data;
	temp_data.f_data = Data;
	for(uint8_t i=0; i<4; i++)
	{
		UsartSendByte(com, temp_data.f_data_byte[i]);
	}
}

uint8_t GetStringLength(const char* string)
{
    uint8_t length = 0;
    while (*string != '\0') 
	{ 
        length++;
        string++;
    }
    return length;
}

#if UART_DMA_ENABLE
void Usart_DMA_TranfDeal(uint16_t ByteCnt, uint32_t SourceAddr)
{
	dma_channel_disable(DMA0, DMA_CH);
	dma_flag_clear(DMA0, DMA_CH, DMA_FLAG_FTF);
	dma_memory_address_config(DMA0, DMA_CH,  (uint32_t)SourceAddr);
	dma_transfer_number_config(DMA0, DMA_CH,  ByteCnt);
	dma_channel_enable(DMA0, DMA_CH);
	/* wait until USART TX DMA0 channel transfer complete */
    while(RESET == dma_flag_get(DMA0, DMA_CH, DMA_INTF_FTFIF));
}
#endif

void Usart_PC_PrintDeal(uint32_t com, uint8_t numofch)
{
	static uint8_t f_size = sizeof(float);
	if(numofch >= Usart_Frame_DataInfo.ch_num)
	{
		numofch = Usart_Frame_DataInfo.ch_num;
	}
#if UART_DMA_ENABLE
	for(uint8_t i=0; i<numofch; i++)
	{
		Usart_DMA_TranfDeal(f_size, (uint32_t)(&(Usart_CH_Frame[i])));
	}
	Usart_DMA_TranfDeal(4, (uint32_t)(&(Usart_CheckTail[0])));


#else
	for(uint8_t i=0; i<numofch; i++)
	{
		for(uint8_t j=0; j<f_size; j++)
		{
			UsartSendByte(com, Usart_Frame_DataInfo.ch_data[i].f_data_byte[j]);
		}
	}
	for(uint8_t i=0; i<Usart_Frame_DataInfo.check_length; i++)
	{
		UsartSendByte(com, Usart_Frame_DataInfo.check_tail[i]);
	}
#endif
}




