#include "BSP_drv8323rs.h"
#include "systick.h"

#if PRINT_MOTHD == USE_C_STANDARD
#include <stdio.h>
#endif
#if PRINT_MOTHD == USE_USART_FUNC
#include "gd32f307c_eval.h"
#endif


uint32_t Drv8323_FaultInfo;
uint16_t drv8323rs_data[4];
uint8_t ErrCode[ERRMAX]={0};

static Cmd_Frame_Drv  Drv_Cmd_Frame;
static Data_Frame_Drv Drv_Receive_Frame;

Dri_Ctl DRI_CTL_DATA;
Gate_HS GATE_HS_DATA;
Gate_LS GATE_LS_DATA;
Ocp_Ctl OCP_CTL_DATA;
Csa_Ctl OSA_CTL_DATA;

/*!
    \brief      configure the nFault Pin EXTI
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void DRV8323_nFAULT_EXTI(void)
{
    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(FAULT_EXTI_PORT_SOURCE, FAULT_EXTI_PIN_SOURCE);

    /* configure key EXTI line */
    exti_init(FAULT_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(FAULT_EXTI_LINE);
}

/*!
    \brief      Generate CMD_R to send
    \param[in]  Adress
    \param[out] uint16_t cmd
    \retval     none
*/
static uint16_t GenCmd_drv(uint8_t Adress, uint8_t RW, uint16_t Data)
{
	Drv_Cmd_Frame.cmd_send_drv = 0u;
	
	Drv_Cmd_Frame.cmd_bit_drv.ADDR = Adress & 0x0F;
	Drv_Cmd_Frame.cmd_bit_drv.RW   = RW;
	if(READ_DATA == Drv_Cmd_Frame.cmd_bit_drv.RW)
	{
		Drv_Cmd_Frame.cmd_bit_drv.DATA = 0;
	}
	else
	{
		Drv_Cmd_Frame.cmd_bit_drv.DATA = Data & 0x7FF;
	}
	return Drv_Cmd_Frame.cmd_send_drv;
}


/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi1_config(void)
{
    spi_parameter_struct spi_init_struct;

    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_16BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_32;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
	spi_init(SPI1, &spi_init_struct);
	ENABLE_DRV8323
	spi_enable(SPI1);
}



/*!
    \brief      Generate CMD_W to send
    \param[in]  Adress
    \param[out] uint16_t cmd
    \retval     none
*/
uint16_t  Drv8323_ReadData(uint8_t address)
{
	Drv_Receive_Frame.data_receive_drv = 0;
	SET_DRV8323_NSS_LOW
	while(RESET == spi_i2s_flag_get(SPI1,SPI_FLAG_TBE)){}
	spi_i2s_data_transmit(SPI1,GenCmd_drv(address, READ_DATA, 0x00));

	while(SET == spi_i2s_flag_get(SPI1,SPI_FLAG_TRANS)){}
	Drv_Receive_Frame.data_receive_drv = spi_i2s_data_receive(SPI1);
	SET_DRV8323_NSS_HIGH
	return Drv_Receive_Frame.data_bit_drv.DATA;
}

uint16_t Drv8323_WriteCmd(uint8_t address,uint16_t cmd)
{
	SET_DRV8323_NSS_LOW
	while(RESET == spi_i2s_flag_get(SPI1,SPI_FLAG_TBE)){}
	spi_i2s_data_transmit(SPI1,GenCmd_drv(address, WRITE_CMD, cmd));

	while(SET == spi_i2s_flag_get(SPI1,SPI_FLAG_TRANS)){}
	Drv_Receive_Frame.data_receive_drv = spi_i2s_data_receive(SPI1);
	
	SET_DRV8323_NSS_HIGH
	return Drv_Receive_Frame.data_bit_drv.DATA;
}

/*!
    \brief      Init DRV8323RS Device
    \param[in]  void
    \param[out] void
    \retval     none
*/
uint16_t DRV8323_Init_Device(void)
{
    uint16_t   Result = 0;
	DRV8323_nFAULT_EXTI();
	DRV8323_DeviceReset();
/*===============================Gate Drive HS===========================================*/
	//Unlock SPI Reg Write
	GATE_HS_DATA.reg.LOCK = 3;
	GATE_HS_DATA.reg.IDRIVEP_HS = 15;
	GATE_HS_DATA.reg.IDRIVEN_HS = 15;
	Drv8323_WriteCmd(Gate_Drive_HS, GATE_HS_DATA.cmd);
	if(GATE_HS_DATA.cmd != Drv8323_ReadData(Gate_Drive_HS))
	{
		Result |= BIT(Gate_Drive_HS);
	}	
/*===============================Driver Control===========================================*/
	DRI_CTL_DATA.reg.DIS_CPUV = 0;
	DRI_CTL_DATA.reg.DIS_GDF  = 0;
	DRI_CTL_DATA.reg.OTW_REP  = 1;
	DRI_CTL_DATA.reg.PWM_MODE = 1;
	DRI_CTL_DATA.reg.COAST    = 0;
	DRI_CTL_DATA.reg.CLR_FLT  = 0;
	Drv8323_WriteCmd(Driver_Control, DRI_CTL_DATA.cmd);
	if(DRI_CTL_DATA.cmd != Drv8323_ReadData(Driver_Control))
	{
		Result |= BIT(Driver_Control);
	}
/*================================Gate Drive LS==========================================*/
	GATE_LS_DATA.reg.CBC = 1;
	GATE_LS_DATA.reg.TDRIVE = 3;
	GATE_LS_DATA.reg.IDRIVEP_LS = 15;
	GATE_LS_DATA.reg.IDRIVEN_LS = 15;
	Drv8323_WriteCmd(Gate_Drive_LS, GATE_LS_DATA.cmd);
	if(GATE_LS_DATA.cmd != Drv8323_ReadData(Gate_Drive_LS))
	{
		Result |= BIT(Gate_Drive_LS);
	}
/*=================================OCP Control=========================================*/
	OCP_CTL_DATA.reg.TRETRY = 0;
	OCP_CTL_DATA.reg.DEADTIME = 1;
	OCP_CTL_DATA.reg.OCP_MODE = 0;
	OCP_CTL_DATA.reg.OCP_DEG = 1;
	OCP_CTL_DATA.reg.VDS_LVL = 9;
	Drv8323_WriteCmd(OCP_Control, OCP_CTL_DATA.cmd);
	if(OCP_CTL_DATA.cmd != Drv8323_ReadData(OCP_Control))
	{
		Result |= BIT(OCP_Control);
	}
/*=================================CSA Control=========================================*/
	OSA_CTL_DATA.reg.CSA_FET = 0;
	OSA_CTL_DATA.reg.LS_REF  = 0;
	OSA_CTL_DATA.reg.VREF_DIV = 1;
	OSA_CTL_DATA.reg.CSA_GAIN = 2; //-7A~7A 有效线性采样区间
	OSA_CTL_DATA.reg.DIS_SEN = 0;
	OSA_CTL_DATA.reg.CSA_CAL_A = 0;
	OSA_CTL_DATA.reg.CSA_CAL_B = 0;
	OSA_CTL_DATA.reg.CSA_CAL_C = 0;
	OSA_CTL_DATA.reg.SEN_LVL = 3;
	Drv8323_WriteCmd(CSA_Control, OSA_CTL_DATA.cmd);
	if(OSA_CTL_DATA.cmd != Drv8323_ReadData(CSA_Control))
	{
		Result |= BIT(CSA_Control);
	}
	
/*================================Coonfigure Over=======================================*/

	return Result;
}

void DRV8323_FaultReset(void)
{
	if(Drv8323_ReadData(Fault_Status1) != 0 || Drv8323_ReadData(Fault_Status2) != 0)
	{
		DRI_CTL_DATA.cmd = Drv8323_ReadData(Driver_Control);
		DRI_CTL_DATA.reg.CLR_FLT = 1;
		Drv8323_WriteCmd(Driver_Control,DRI_CTL_DATA.cmd);
	}
}

void DRV8323_DeviceReset(void)
{
	DISABLE_DRV8323;
	delay_1ms(1);
	ENABLE_DRV8323;
}

void DRV8323_Init_SYSTEM(void)
{
	uint16_t Result = DRV8323_Init_Device();
	uint16_t SysErr = 0;
	if( 0 == Result)
	{
		SysErr = 0;
	}
	else
	{
		if(Result&BIT(Driver_Control))
		{
			Drv8323_WriteCmd(Driver_Control, DRI_CTL_DATA.cmd);
			if(DRI_CTL_DATA.cmd != Drv8323_ReadData(Driver_Control))
			{
				SysErr |= BIT(Driver_Control);
			}
			else{}
		}
		if(Result&BIT(Gate_Drive_HS))
		{
			Drv8323_WriteCmd(Gate_Drive_HS, GATE_HS_DATA.cmd);
			if(GATE_HS_DATA.cmd != Drv8323_ReadData(Gate_Drive_HS))
			{
				SysErr |= BIT(Gate_Drive_HS);
			}
			else{}
		}
		if(Result&BIT(Gate_Drive_LS))
		{
			Drv8323_WriteCmd(Gate_Drive_LS, GATE_LS_DATA.cmd);
			if(GATE_LS_DATA.cmd != Drv8323_ReadData(Gate_Drive_LS))
			{
				SysErr |= BIT(Gate_Drive_LS);
			}
			else{}		
		}
		if(Result&BIT(OCP_Control))
		{
			Drv8323_WriteCmd(OCP_Control, OCP_CTL_DATA.cmd);
			if(OCP_CTL_DATA.cmd != Drv8323_ReadData(OCP_Control))
			{
				SysErr |= BIT(OCP_Control);
			}
			else{}
		}	
		if(Result&BIT(CSA_Control))
		{
			Drv8323_WriteCmd(CSA_Control, OSA_CTL_DATA.cmd);
			if(OSA_CTL_DATA.cmd != Drv8323_ReadData(CSA_Control))
			{
				SysErr |= BIT(CSA_Control);
			}
			else{}	
		}
		else
		{
			SysErr = 0xFFFF;
		}
	}
	
	if(SysErr == 0)
	{
#if PRINT_MOTHD == USE_C_STANDARD
		printf("DRV8323 INIT SUCCESS!\n");
#endif
#if PRINT_MOTHD == USE_USART_FUNC
		UsartSendStr(USART1,"DRV8323 INIT SUCCESS!\n");
#endif

	}
	else
	{
#if PRINT_MOTHD == USE_C_STANDARD
		printf("CURRENT ERR: %d\n",SysErr);
#endif
#if PRINT_MOTHD == USE_USART_FUNC
		UsartSendStr(USART1,"CURRENT ERR:");
		UsartSendUint16(USART1,SysErr);
		UsartSendStr(USART1,"\n");
#endif

	}

}

void Report_Drv8323_FaultInfo(void)
{	
	uint16_t ErrCnt = 0;
	Drv8323_FaultInfo = (((uint32_t)Drv8323_ReadData(Fault_Status1))<<11) | (((uint32_t)Drv8323_ReadData(Fault_Status2))<<0);
	for(int i=0;i<GEN_FAULT;i++)
	{
		if(Drv8323_FaultInfo & BIT(i))
		{
			ErrCode[ErrCnt] = i;
			ErrCnt++;
		}
	}
	for(int i=0; i<ErrCnt;i++)
	{
#if PRINT_MOTHD == USE_C_STANDARD
		printf("ErrCode is %d\n",ErrCode[i]);
#endif
#if PRINT_MOTHD == USE_USART_FUNC
		UsartSendStr(USART1,"ErrCode is:");
		UsartSendByte(USART1,ErrCode[i]);
		UsartSendStr(USART1,"\n");
#endif
		
	}
}



