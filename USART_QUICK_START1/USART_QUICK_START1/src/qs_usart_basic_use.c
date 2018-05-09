//#define CALIBRATE

#include <asf.h>

#include "stdio.h"
#include "UARTsetup.h"
#include "SPI_setup.h"
#include "AD7192.h"
#include "bumpers.h"
#include "timer.h"

enum state{
	ZEROSCALING,
	SENDINGDATA
} state;

enum drone_state{
	AIRBORN,
	LANDED
} drone_state;

int64_t data = 0;
char recived_data = 0;
int64_t zoffset = 0;

void loop();

int main(void)
{
		
	system_init();

	for (uint16_t i = 0; i < DATA_COLLECTED; i++)
	{
		col_data[i] = 0;
	}

	configure_usart();
	//configure_usart_callbacks();
	configure_spi_master();
	configure_AD7192();
	
	#ifndef CALIBRATE
	configure_tc();
	configure_tc_callbacks();
	#endif //CALIBRATE

	system_interrupt_enable_global();

	//rintln("Testing start", sizeof("Testing start"));
	state = ZEROSCALING;

	int8_t uart_read_data[16];
	drone_state = LANDED;
	while (1) {
		
		loop();
		usart_read_wait(&usart_instance, uart_read_data);
		if (uart_read_data[0] == 'r')
		{
			state = ZEROSCALING;
		}
	}
	return 0;
}

void loop() {
	if(state == ZEROSCALING) {
		zoffset = 0;
		for(uint16_t i = 0; i < DATA_COLLECTED; i++) {
			col_data[i] = get_data_from_landingsensor(1);
			zoffset += col_data[i];
		}
		for(uint16_t i = 0; i < DATA_COLLECTED; i++) {
			col_data[i] -= zoffset;
		}
		zoffset = zoffset/DATA_COLLECTED;
		state = SENDINGDATA;
	} else if (state == SENDINGDATA)
	{
		data = get_data_from_landingsensor(1) - zoffset;
		data = moving_average(data);

		#ifdef CALIBRATE
		printint(data);
		//printint(get_data_from_landingsensor(0));
		//print("\t", 1);
		//printint(get_data_from_landingsensor(1));
		print("\n",	1);
		#endif // CALIBRATE;
	
		#ifndef CALIBRATE
		if(data > 1200000) { //airborn
			//if(bumpers_is_pushed()) { //bumper activated
			if(drone_state == AIRBORN) {
				tc_set_data_to_send('a');
				
			} else {		//just landed
				print("a", 1); 
				drone_state = AIRBORN;
			}
			//setBufData('l');
		} 
		else if (data != 0) //landed
		{
			if(drone_state == LANDED) {
				tc_set_data_to_send('l');
				
			} 
			else {		  //just landed
				print("l", 1);
				drone_state = LANDED;
			}
		} 
		else { //error
			tc_set_data_to_send('e');
			
		}
		#endif // CALIBRATE
			
	}
}
