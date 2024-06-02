#include "BSP_as5047p.h"
#include "systick.h"

float RtrAngle;

Cmd_Frame AS5047_CMD;
Data_Frame AS5047_DATA;


/*!
    \brief      configure the SPI peripheral
    \param[in]  Frame to check
    \param[out] EVEN check result
    \retval     none
*/
static uint16_t SPI_EVEN(uint16_t Frame)
{
	uint16_t cnt1 = 0;
	uint16_t cnt2 = 0;
	uint16_t result = 0;
	while(((Frame >> cnt1) & 0xFFFF) && (cnt1 < EVEN_CHECK_LEN))
	{
		if((Frame >> cnt1)  & ((uint16_t)0x01))
		{
			cnt2++;
		}
		cnt1++;
	}
	if((cnt2 % 2) == 0)
	{
		result = 0;
	}
	else
	{
		result = 1;
	}
	return result;
}

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi2_config(void)
{
    spi_parameter_struct spi_init_struct;
    /* SPI2 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_16BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_32;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
	spi_init(SPI2, &spi_init_struct);
	spi_enable(SPI2);

}

/*!
    \brief      Generate CMD_R to send
    \param[in]  Adress
    \param[out] uint16_t cmd
    \retval     none
*/
uint16_t GenCmd2Read(uint16_t Adress)
{
	AS5047_CMD.cmd_send = 0u;
	
	AS5047_CMD.cmd_bit.ADDR = Adress & 0x3FFF;
	AS5047_CMD.cmd_bit.RW   = READ_DATA;
	AS5047_CMD.cmd_bit.PARC = SPI_EVEN(AS5047_CMD.cmd_send);
	return AS5047_CMD.cmd_send;
}

/*!
    \brief      Generate CMD_W to send
    \param[in]  Adress
    \param[out] uint16_t cmd
    \retval     none
*/
uint16_t GenCmd2Write(uint16_t Command)
{
	AS5047_CMD.cmd_send = 0u;
	
	AS5047_CMD.cmd_bit.ADDR = Command & 0x3FFF;
	AS5047_CMD.cmd_bit.RW   = WRITE_CMD;
	AS5047_CMD.cmd_bit.PARC = SPI_EVEN(AS5047_CMD.cmd_send);
	
	return AS5047_CMD.cmd_send;
}

/*!
    \brief      Generate CMD_W to send
    \param[in]  Adress
    \param[out] uint16_t cmd
    \retval     Angle at Last Step
    Read---Read---Read---Read---Read
    	 \	    \	   \	  \
    	  \	     \	    \	   \
    	   Angle--Angle--Angle--Anglen
*/
uint16_t  AS5047_GetAngle(void)
{
	uint16_t angle;
	AS5047_DATA.data_receive = 0;
	SET_SPI2_NSS_LOW
	while(RESET == spi_i2s_flag_get(SPI2,SPI_FLAG_TBE)){}
	spi_i2s_data_transmit(SPI2,GenCmd2Read(AS5047_ANGLECOM));

	while(SET == spi_i2s_flag_get(SPI2,SPI_FLAG_TRANS)){}
	AS5047_DATA.data_receive = spi_i2s_data_receive(SPI2);
	
	angle = (uint16_t)AS5047_DATA.data_bit.DATA;
	SET_SPI2_NSS_HIGH
	return angle;
}






