/*
 * button_landing.c
 *
 * Created: 04.06.2018 19:35:29
 *  Author: EdvardOlaf
 */ 
 #include "button_landing.h"

#define LANDING_BUTTON1_PIN PORT_PB08
#define LANDING_BUTTON1_HIGH_PIN PORT_PB09

#define LANDING_BUTTON2_PIN PORT_PA27
#define LANDING_BUTTON2_HIGH_PIN PORT_PB23

#define LANDING_BUTTON3_PIN PORT_PA28
#define LANDING_BUTTON3_HIGH_PIN PORT_PA13

#define LANDING_BUTTON4_PIN PORT_PA12
#define LANDING_BUTTON4_HIGH_PIN PORT_PB11


void init_button_landing() {
	//Set up pull-down
	PORT->Group[1].PINCFG[8].reg = PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[27].reg =  PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[28].reg =  PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[12].reg =  PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;

	PORT->Group[0].DIRCLR.reg = LANDING_BUTTON2_PIN | LANDING_BUTTON3_PIN | LANDING_BUTTON4_PIN;
	PORT->Group[0].OUTCLR.reg = LANDING_BUTTON2_PIN | LANDING_BUTTON3_PIN | LANDING_BUTTON4_PIN;

	PORT->Group[1].DIRCLR.reg = LANDING_BUTTON1_PIN;
	PORT->Group[1].OUTCLR.reg = LANDING_BUTTON1_PIN;


	//set pins to GND
	PORT->Group[0].DIRSET.reg = LANDING_BUTTON3_HIGH_PIN;
	PORT->Group[0].OUTSET.reg = LANDING_BUTTON3_HIGH_PIN;

	PORT->Group[1].DIRSET.reg = LANDING_BUTTON2_HIGH_PIN | LANDING_BUTTON1_HIGH_PIN | LANDING_BUTTON4_HIGH_PIN;
	PORT->Group[1].OUTSET.reg = LANDING_BUTTON2_HIGH_PIN | LANDING_BUTTON1_HIGH_PIN | LANDING_BUTTON4_HIGH_PIN;

}

int read_button_landing(int landing_button_index) {
	if(landing_button_index == 1) {
		return (PORT->Group[1].IN.reg & LANDING_BUTTON1_PIN) != 0;
	} else if(landing_button_index == 2) {
		return (PORT->Group[0].IN.reg & LANDING_BUTTON2_PIN) != 0;
	} else if(landing_button_index == 3) {
		return (PORT->Group[0].IN.reg & LANDING_BUTTON3_PIN) != 0;
	}else if(landing_button_index == 4) {
		return (PORT->Group[0].IN.reg & LANDING_BUTTON4_PIN) != 0;
	} else {
		return 1;
	}

}

int number_of_triggered_landing_buttons() {
	int count = 0;
	for(int i = 1; i <= 4; i++) {
		count += read_button_landing(i);
	}
	return count;
} 

int landing_buttons_triggerd() {
	return ((PORT->Group[0].IN.reg &(LANDING_BUTTON2_PIN | LANDING_BUTTON3_PIN | LANDING_BUTTON4_PIN)) != 0) || (PORT->Group[0].IN.reg &(LANDING_BUTTON1_PIN) != 0); 
}