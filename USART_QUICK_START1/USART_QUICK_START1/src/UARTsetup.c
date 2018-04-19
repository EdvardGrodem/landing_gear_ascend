#include "UARTsetup.h"


char tx_data = 'a';


void usart_read_callback(struct usart_module *const usart_module)
{
	if(rx_buffer[0] == 'n' ) {
		rx_buffer[0] = (uint8_t)tx_data;
	} else if(rx_buffer[0] == 'z'){
		rx_buffer[0] = (uint8_t)'r';
	} else {
		rx_buffer[0] = (uint8_t)'e';
	}
	usart_write_buffer_job(&usart_instance,(uint8_t *)rx_buffer, MAX_RX_BUFFER_LENGTH);
}
void usart_write_callback(struct usart_module *const usart_module)
{
	port_pin_toggle_output_level(LED_0_PIN);
}

void configure_usart(void) {

	struct usart_config config_usart;

	usart_get_config_defaults(&config_usart);

	config_usart.baudrate    = 9600; //115200;
	config_usart.mux_setting = MY_SERCOM_MUX_SETTING;
	config_usart.pinmux_pad0 = MY_SERCOM_PINMUX_PAD0;
	config_usart.pinmux_pad1 = MY_SERCOM_PINMUX_PAD1;
	config_usart.pinmux_pad2 = MY_SERCOM_PINMUX_PAD2;
	config_usart.pinmux_pad3 = MY_SERCOM_PINMUX_PAD3;

	while (usart_init(&usart_instance,
	MY_MODULE, &config_usart) != STATUS_OK) {
	}
	usart_enable(&usart_instance);
	
}
void configure_usart_callbacks(void)
{
	usart_register_callback(&usart_instance,
	usart_write_callback, USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_register_callback(&usart_instance,
	usart_read_callback, USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&usart_instance,
	USART_CALLBACK_BUFFER_TRANSMITTED);
	usart_enable_callback(&usart_instance, USART_CALLBACK_BUFFER_RECEIVED);
}

void print(uint8_t * string, uint16_t size) {
	usart_write_buffer_wait(&usart_instance, string, size);
}
void println(uint8_t * string, uint16_t size) {
	usart_write_buffer_wait(&usart_instance, string, size);
	usart_write_buffer_wait(&usart_instance, "\r\n", sizeof("\r\n"));
}
void printint(uint32_t integer) {
	int size = 10; //(log10(integer))+1;
	char string[] = "          ";
	sprintf(string, "%d", integer);
	print(string, size);
}

void setBufData(char data) {
	tx_data = data;
}