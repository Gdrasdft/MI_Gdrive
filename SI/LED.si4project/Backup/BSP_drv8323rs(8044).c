#include "BSP_drv8323rs.h"
#include "systick.h"

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
    /* enable the key clock */
    rcu_periph_clock_enable(GPIO_CLK_FAULT);
    rcu_periph_clock_enable(RCU_AF);

    /* configure button pin as input */
    gpio_init(GPIO_PORT_FAULT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_FAULT);

    /* enable and set nFAULT EXTI interrupt to the lowest priority */
    nvic_irq_enable(FAULT_EXTI_IRQn, 2U, 0U);

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
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_SPI1);

    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13 | GPIO_PIN_15);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    /* PA3 as NSS */
	gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);

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
    \brief      Init DRV8323RS Device
    \param[in]  void
    \param[out] void
    \retval     none
*/
uint16_t DRV8323_Init_Device(void)
{
    uint16_t   Result = 0;
	DRV8323_nFAULT_EXTI();

	//Unlock SPI Reg Write	
	GATE_HS_DATA.reg.LOCK = 6;
	GATE_HS_DATA.reg.IDRIVEP_HS = 15;
	GATE_HS_DATA.reg.IDRIVEN_HS = 15;
	Result = Drv8323_WriteCmd(Gate_Drive_HS, GATE_HS_DATA.cmd);

	/*DRI_CTL_DATA.reg.DIS_CPUV = 1;
	DRI_CTL_DATA.reg.DIS_GDF  = 1;
	DRI_CTL_DATA.reg.OTW_REP  = 0;
	DRI_CTL_DATA.reg.PWM_MODE = 1;
	DRI_CTL_DATA.reg.COAST    = 1;
	DRI_CTL_DATA.reg.CLR_FLT  = 0;
	Result = Drv8323_WriteCmd(Driver_Control, DRI_CTL_DATA.cmd);

	GATE_LS_DATA.reg.CBC = 1;
	GATE_LS_DATA.reg.TDRIVE = 3;
	GATE_LS_DATA.reg.IDRIVEP_LS = 15;
	GATE_LS_DATA.reg.IDRIVEN_LS = 15;
	Result = Drv8323_WriteCmd(Gate_Drive_HS, GATE_LS_DATA.cmd);*/
	
	return Result;
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



