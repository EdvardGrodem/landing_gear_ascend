#include "AD7192.h"
#include "SPI_setup.h"


uint64_t ADC_wheight[2] = {1, 1};

void configure_AD7192() {

	//Configurate ADC1
	send_data(0xffffffffff, 1, 5);
	uint32_t adc_config = ACX_EN | 	AIN1_AIN2/*| AIN3_AIN4*/ | BUF_EN | GAIN_128 | REFDET_EN | CHOP_EN; //configure input
	set_register_to(1, 0b010, adc_config, 3);

	uint32_t adc_mode = MODE_SYSZCL| INCLK_MCLK2TRI; //Zero scale calibration
	set_register_to(1, 0b001, adc_mode, 3);

	while((PORT->Group[0].IN.reg & SPI1_MISO_PIN)) {}

	adc_mode = MODE_CONT| INCLK_MCLK2TRI | DAT_STA_DIS;
	set_register_to(1, 0b001, adc_mode, 3);

	//Configurate ADC0
	send_data(0xffffffffff, 0, 5);
	adc_config = ACX_EN | 	AIN1_AIN2 /*| AIN3_AIN4*/ |BUF_EN | GAIN_128 | REFDET_EN | CHOP_EN; //configure input
	set_register_to(0, 0b010, adc_config, 3);
	
	adc_mode = MODE_SYSZCL| INCLK_MCLK2TRI; //Zero scale calibration
	set_register_to(0, 0b001, adc_mode, 3);

	while((PORT->Group[0].IN.reg & SPI1_MISO_PIN)) {}

	adc_mode = MODE_CONT| INCLK_MCLK2TRI | DAT_STA_DIS; //Set to normal settings
	set_register_to(0, 0b001, adc_mode, 3);
}

void set_register_to(int ADC_index, uint64_t reg_adress, uint64_t value, uint64_t bytes_in_register) {
	reg_adress = (reg_adress << 3);
	send_data(reg_adress,ADC_index, 1);
	send_data(value, ADC_index, bytes_in_register);
}

void get_register_data_at(int ADC_index, uint64_t reg_adress, uint64_t * data, uint64_t bytes_in_register){
	*data = 0;
	reg_adress = (reg_adress << 3) | (1 << 6);
	send_data(reg_adress, ADC_index, 1);
	read_data(data,ADC_index, bytes_in_register);
}

uint64_t return_register_data_at(int ADC_index, uint64_t reg_adress, uint64_t bytes_in_register) {
	uint64_t data;
	get_register_data_at(ADC_index, reg_adress, &data, bytes_in_register);
	return data;
}


void nois_reduction(int64_t * data, int64_t * zoffset, int ADC_index) {
	uint64_t * col_data;
	if(ADC_index) {
		col_data = col_data_ADC1;
	}
	else {
		col_data = col_data_ADC0;
	}
	*data -= *zoffset;
	if(*data > 600000 || *data < -600000) {
		*data = col_data[0];
	}
	
	for (uint16_t i = DATA_COLLECTED-1; i > 0; i--)
	{
		col_data[i] = col_data[i-1];
	}
	col_data[0] = *data;
	int64_t weight = 0;
	for (uint16_t i= 1; i < DATA_COLLECTED; i++)
	{
		weight += (DATA_COLLECTED-i);
		*data += col_data[i]*(DATA_COLLECTED-i);
	}
	*data = (*data)/weight;
}
int64_t moving_average(uint64_t data) {
	for (uint16_t i = DATA_COLLECTED-1; i > 0; i--)
	{
		col_data[i] = col_data[i-1];
	}
	col_data[0] = data;
	int64_t weight = 0;
	for (uint16_t i= 1; i < DATA_COLLECTED; i++)
	{
		//weight += (DATA_COLLECTED-i);
		data += col_data[i];
	}
	return data;
}


void start_continous_read() {
	send_data(0x01011100, 1, 1);
	send_data(0x01011100, 0, 1);
}

void read_all_channels(int64_t * data) {
	int64_t databuffer = 0;
	for (int adcindexi = 0; adcindexi < 2; adcindexi++)
	{
		for (int channelindex = 0; channelindex < 2; channelindex++)
		{
			read_data(databuffer, adcindexi, 4);
			data[2*adcindexi+databuffer&0b111] = databuffer>>8;
		}
	}

}

void stop_continous_read() {
	send_data(0x01011000, 1, 1);
	send_data(0x01011000, 0, 1);
}

uint64_t get_data_from_landingsensor(int ADC_index) {
	uint64_t temp_data = 0;
	get_register_data_at(ADC_index, 3, &temp_data, 3);
	temp_data *= ADC_wheight[ADC_index];
	if(ADC_index) {
		temp_data *= -1;
	}
	return temp_data;
}
