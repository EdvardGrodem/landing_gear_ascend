//#define CALIBRATE

#include <asf.h>

#include "stdio.h"
#include "UARTsetup.h"
#include "bumpers.h"
#include "timer.h"
#include "bumpers.h"
#include "button_landing.h"

#define LED_1_PIN PORT_PA20 // D1 lyser ved landing

enum drone_state{
	AIRBORN,
	LANDED
} drone_state;

void loop();

int main(void)
{	
	system_init();
	configure_usart();

	#ifndef CALIBRATE
	configure_tc();	//modul som sender understell ca 10 ganger i sekundet
	configure_tc_callbacks();
	#endif //CALIBRATE

	init_bumpers();
	init_button_landing();
	system_interrupt_enable_global();

	PORT->Group[0].DIRSET.reg |= LED_1_PIN;

	drone_state = LANDED;
	while (1) {
		loop();
	}
	return 0;
}

void loop() {
	
		#ifdef CALIBRATE
		printint(number_of_triggered_landing_buttons());
		print("\t", 1);
		for(int i = 1; i <= 4; i++) {
			printint(read_button_landing(i));
			print("\t", 1);
		}
		printint(bumpers_is_pushed());
		print("\t", 1);
		printint(PORT->Group[0].IN.reg);
		print("\t", 1);
		print("\n",	1);
		#endif // CALIBRATE;
	
		#ifndef CALIBRATE
		if(number_of_triggered_landing_buttons() >= 2) { //Landed
			
			if(drone_state == LANDED) {
				if(bumpers_is_pushed()) {
					tc_set_data_to_send('b');
				} else {
					tc_set_data_to_send('l');
				}
		} else {
				drone_state = LANDED;
				print("1", 1);
				//PORT->Group[0].OUTSET.reg |= LED_1_PIN;
				//av ukjent grunn fungerer ikke denne sammen med landingssensor3
			}
		} 
		else //airborn
		{	
			drone_state = AIRBORN;
			tc_set_data_to_send('a');
			//PORT->Group[0].OUTCLR.reg |= LED_1_PIN;	
		} 
		#endif // CALIBRATE
}
