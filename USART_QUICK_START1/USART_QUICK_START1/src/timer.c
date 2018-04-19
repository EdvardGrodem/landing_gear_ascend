/*
 * timer.c
 *
 * Created: 07.04.2018 16:07:13
 *  Author: EdvardOlaf
 */ 
 #include "timer.h"

 #define CONF_TC_MODULE TC3

 char tc_data = 'e';

 void tc_callback_to_send_data(
 struct tc_module *const module_inst)
 {
	 print(&tc_data, 1);
 }

 void configure_tc(void)
 {
	 struct tc_config config_tc;
	 tc_get_config_defaults(&config_tc);
	 config_tc.counter_size = TC_COUNTER_SIZE_8BIT;
	 config_tc.clock_source = GCLK_GENERATOR_2;
	 config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV16;
	 config_tc.counter_8_bit.period = 50;
	 tc_init(&tc_instance, CONF_TC_MODULE, &config_tc);
	 tc_enable(&tc_instance);
 }

 void configure_tc_callbacks(void)
 {
	 tc_register_callback(&tc_instance, tc_callback_to_send_data,
	 TC_CALLBACK_OVERFLOW);
	 tc_enable_callback(&tc_instance, TC_CALLBACK_OVERFLOW);
 }

 void tc_set_data_to_send(char tc_data_p) {
	tc_data = tc_data_p;
 }