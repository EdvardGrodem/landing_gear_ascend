/*
 * timer.h
 *
 * Created: 07.04.2018 16:07:31
 *  Author: EdvardOlaf
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "UARTsetup.h"

struct tc_module tc_instance;


 void tc_callback_to_send_data(struct tc_module *const module_inst);
 void configure_tc(void);
 void configure_tc_callbacks(void);
 void tc_set_data_to_send(char c);


#endif /* TIMER_H_ */