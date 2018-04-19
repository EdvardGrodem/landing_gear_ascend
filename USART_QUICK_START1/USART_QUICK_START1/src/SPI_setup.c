#include "SPI_setup.h"

void configure_spi_master(void)
{
	struct spi_config config_spi_master;

	spi_get_config_defaults(&config_spi_master);
	config_spi_master.mux_setting = SPI1_MUX_SETTING;
	config_spi_master.pinmux_pad0 = SPI1_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = SPI1_PINMUX_PAD1;
	config_spi_master.pinmux_pad2 = SPI1_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = PINMUX_UNUSED;
	config_spi_master.transfer_mode = SPI_TRANSFER_MODE_3;
	config_spi_master.generator_source = GCLK_GENERATOR_1;
	config_spi_master.data_order = SPI_DATA_ORDER_MSB;
	spi_init(&spi1_master_instance, SPI1_MODULE, &config_spi_master);
	spi_set_baudrate(&spi1_master_instance, SPI_BAUDRATE);
	//SERCOM1->SPI.INTENSET.reg = SERCOM_SPI_INTENSET_TXC;

	spi_enable(&spi1_master_instance);

	PORT->Group[0].DIRSET.reg = SLAVE1_SELECT_PIN;
	PORT->Group[0].OUTSET.reg = SLAVE1_SELECT_PIN;


	spi_get_config_defaults(&config_spi_master);
	config_spi_master.mux_setting = SPI0_MUX_SETTING;
	config_spi_master.pinmux_pad0 = SPI0_PINMUX_PAD0;
	config_spi_master.pinmux_pad1 = SPI0_PINMUX_PAD1;
	config_spi_master.pinmux_pad2 = SPI0_PINMUX_PAD2;
	config_spi_master.pinmux_pad3 = PINMUX_UNUSED;
	config_spi_master.transfer_mode = SPI_TRANSFER_MODE_3;
	config_spi_master.generator_source = GCLK_GENERATOR_1;
	config_spi_master.data_order = SPI_DATA_ORDER_MSB;
	spi_init(&spi0_master_instance, SPI0_MODULE, &config_spi_master);
	spi_set_baudrate(&spi0_master_instance, SPI_BAUDRATE);
	//SERCOM0->SPI.INTENSET.reg = SERCOM_SPI_INTENSET_TXC;

	spi_enable(&spi0_master_instance);

	PORT->Group[0].DIRSET.reg = SLAVE0_SELECT_PIN;
	PORT->Group[0].OUTSET.reg = SLAVE0_SELECT_PIN;

	//NVIC_EnableIRQ(SERCOM0_IRQn);
	//NVIC_EnableIRQ(SERCOM1_IRQn);
}

void send_data(int64_t data, int ADC_index, uint64_t bytes){
	if(ADC_index) {
		PORT->Group[0].OUTCLR.reg = SLAVE1_SELECT_PIN;
		//SERCOM1->SPI.INTENCLR.reg = SERCOM_SPI_INTENCLR_TXC;
		for(int i = bytes; i > 0; i--) {
			while(spi_is_ready_to_write(&spi1_master_instance) != true) {}
			spi_write(&spi1_master_instance, data>>(i-1)*8);
			while (spi_is_ready_to_read(&spi1_master_instance) != true) {}
			spi_read(&spi1_master_instance, NULL);
		}
		while(spi_is_ready_to_write(&spi1_master_instance) != true) {}
		PORT->Group[0].OUTSET.reg = SLAVE1_SELECT_PIN;
		//SERCOM1->SPI.INTENSET.reg = SERCOM_SPI_INTENSET_TXC;

	}
	else {
		PORT->Group[0].OUTCLR.reg = SLAVE0_SELECT_PIN;
		//SERCOM0->SPI.INTENCLR.reg = SERCOM_SPI_INTENCLR_TXC;
		for(int i = bytes; i > 0; i--) {
			while(spi_is_ready_to_write(&spi0_master_instance) != true) {}
			spi_write(&spi0_master_instance, data>>(i-1)*8);
			while (spi_is_ready_to_read(&spi0_master_instance) != true) {}
			spi_read(&spi0_master_instance, NULL);
		}
		while(spi_is_ready_to_write(&spi0_master_instance) != true) {}
		PORT->Group[0].OUTSET.reg = SLAVE0_SELECT_PIN;
		//SERCOM0->SPI.INTENSET.reg = SERCOM_SPI_INTENSET_TXC;
	}
}

void read_data(int64_t * data, int ADC_index, uint64_t bytes){
	uint64_t i;
	uint64_t shiftedData;
	if(ADC_index) {
		while((PORT->Group[0].IN.reg & SPI1_MISO_PIN)==1 || (PORT->Group[0].OUT.reg & SLAVE1_SELECT_PIN)==0) {}
		PORT->Group[0].OUTCLR.reg = SLAVE1_SELECT_PIN;
		while((PORT->Group[0].IN.reg & SPI1_MISO_PIN)) {}
		//SERCOM1->SPI.INTENCLR.reg = SERCOM_SPI_INTENCLR_TXC;
		for(i = bytes; i > 0; i--) {
			shiftedData = 0;
			while(!spi_is_ready_to_write(&spi1_master_instance));
			spi_write(&spi1_master_instance, 0b00000000);
			while(!spi_is_ready_to_read(&spi1_master_instance));
			spi_read(&spi1_master_instance, &shiftedData);
			*data |= shiftedData << (i-1)*8;
		}
		while(spi_is_ready_to_write(&spi0_master_instance) != true) {}
		PORT->Group[0].OUTSET.reg = SLAVE1_SELECT_PIN;
		//SERCOM1->SPI.INTENSET.reg = SERCOM_SPI_INTENSET_TXC;
	} else {
		while((PORT->Group[0].IN.reg & SPI0_MISO_PIN)==1 || (PORT->Group[0].OUT.reg & SLAVE0_SELECT_PIN)==0) {}
		PORT->Group[0].OUTCLR.reg = SLAVE0_SELECT_PIN;
		while((PORT->Group[0].IN.reg & SPI0_MISO_PIN)) {}
		//SERCOM0->SPI.INTENCLR.reg = SERCOM_SPI_INTENCLR_TXC;
		for(i = bytes; i > 0; i--) {
			shiftedData = 0;
			while(!spi_is_ready_to_write(&spi0_master_instance));
			spi_write(&spi0_master_instance, 0b00000000);
			while(!spi_is_ready_to_read(&spi0_master_instance));
			spi_read(&spi0_master_instance, &shiftedData);
			*data |= shiftedData << (i-1)*8;
		}
		//SERCOM0->SPI.INTENSET.reg = SERCOM_SPI_INTENSET_TXC;
		while(spi_is_ready_to_write(&spi0_master_instance) != true) {}
		PORT->Group[0].OUTSET.reg = SLAVE0_SELECT_PIN;
	}
}
