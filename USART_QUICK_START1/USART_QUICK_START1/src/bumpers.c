
#include "bumpers.h"

#define LEFT_BUMPER_PIN PORT_PA03
#define LEFT_BUMPER_HIGH_PIN PORT_PA01

#define RIGHT_BUMPER_PIN PORT_PA21
#define RIGHT_BUMPER_HIGH_PIN PORT_PA22

#define FRONT_BUMPER_PIN PORT_PA06
#define FRONT_BUMPER_HIGH_PIN PORT_PA07

#define BACK_BUMPER_PIN PORT_PA05
#define BACK_BUMPER_HIGH_PIN PORT_PA04


void init_bumpers() {
	//Set up pull-down
	PORT->Group[0].PINCFG[5].reg = PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[21].reg =  PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[6].reg =  PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
	PORT->Group[0].PINCFG[3].reg =  PORT_PINCFG_PULLEN | PORT_PINCFG_INEN;
	PORT->Group[0].DIRCLR.reg = LEFT_BUMPER_PIN | RIGHT_BUMPER_PIN | FRONT_BUMPER_PIN | BACK_BUMPER_PIN;
	PORT->Group[0].OUTCLR.reg =  LEFT_BUMPER_PIN | RIGHT_BUMPER_PIN | FRONT_BUMPER_PIN | BACK_BUMPER_PIN;

	//set pins to GND
	PORT->Group[0].DIRSET.reg = LEFT_BUMPER_HIGH_PIN | RIGHT_BUMPER_HIGH_PIN | FRONT_BUMPER_HIGH_PIN | BACK_BUMPER_HIGH_PIN;
	PORT->Group[0].OUTSET.reg = LEFT_BUMPER_HIGH_PIN | RIGHT_BUMPER_HIGH_PIN | FRONT_BUMPER_HIGH_PIN | BACK_BUMPER_HIGH_PIN;

}

int read_bumper(enum bumpers_enum bumper) {
	if(bumper == FRONT) {
		return (PORT->Group[0].IN.reg & FRONT_BUMPER_PIN) != 0;
	} else if(bumper == BACK) {
		return (PORT->Group[0].IN.reg & BACK_BUMPER_PIN) != 0;
	} else if(bumper == LEFT) {
		return (PORT->Group[0].IN.reg & LEFT_BUMPER_PIN) != 0;
	}else if(bumper == RIGHT) {
		return (PORT->Group[0].IN.reg & RIGHT_BUMPER_PIN) != 0;
	} else {
		return 1;
	}

}

int bumpers_is_pushed() {
	return (PORT->Group[0].IN.reg &(LEFT_BUMPER_PIN | RIGHT_BUMPER_PIN | FRONT_BUMPER_PIN | BACK_BUMPER_PIN)) != 0;
}
