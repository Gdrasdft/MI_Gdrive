#include "BSP_drv8323rs.h"
#include "systick.h"

Cmd_Frame_Drv  Drv_Cmd_Frame;
Data_Frame_Drv Drv_Receive_Frame;


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
    \brief      Generate CMD_R to send
    \param[in]  Adress
    \param[out] uint16_t cmd
    \retval     none
*/
uint16_t GenCmd_drv(uint8_t Adress, uint8_t RW, uint16_t Data)
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
		Drv_Cmd_Frame.cmd_bit_drv.DATA = Data;
	}
	return Drv_Cmd_Frame.cmd_send_drv;
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
	return Drv_Receive_Frame.data_receive_drv;
}

uint16_t Drv8323_WriteCmd(uint8_t address,uint16_t cmd)
{
	SET_DRV8323_NSS_LOW
	while(RESET == spi_i2s_flag_get(SPI1,SPI_FLAG_TBE)){}
	spi_i2s_data_transmit(SPI1,GenCmd_drv(address, WRITE_CMD, cmd));

	while(SET == spi_i2s_flag_get(SPI1,SPI_FLAG_TRANS)){}
	Drv_Receive_Frame.data_receive_drv = spi_i2s_data_receive(SPI1);
	
	SET_DRV8323_NSS_HIGH
	return Drv_Receive_Frame.data_receive_drv;
}



