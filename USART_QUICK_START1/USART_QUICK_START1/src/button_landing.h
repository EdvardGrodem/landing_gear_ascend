/*
 * button_landing.h
 *
 * Created: 04.06.2018 19:35:04
 *  Author: EdvardOlaf
 */ 


#ifndef BUTTON_LANDING_H_
#define BUTTON_LANDING_H_
#include "asf.h"

void init_button_landing();

int read_button_landing(int landing_button_index);
int number_of_triggered_landing_buttons();
int landing_buttons_triggerd();


#endif /* BUTTON_LANDING_H_ */