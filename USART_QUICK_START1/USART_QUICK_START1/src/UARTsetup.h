
#ifndef UARTSETUP_H_
#define UARTSETUP_H_

#include "asf.h"
#include <stdio.h>


#define MY_MODULE              SERCOM5
#define MY_SERCOM_MUX_SETTING  USART_RX_1_TX_0_XCK_1
#define MY_SERCOM_PINMUX_PAD0  PINMUX_PB02D_SERCOM5_PAD0
#define MY_SERCOM_PINMUX_PAD1  PINMUX_PB03D_SERCOM5_PAD1
#define MY_SERCOM_PINMUX_PAD2  PINMUX_UNUSED
#define MY_SERCOM_PINMUX_PAD3  PINMUX_UNUSED
#define MY_SERCOM_DMAC_ID_TX   SERCOM5_DMAC_ID_TX
#define MY_SERCOM_DMAC_ID_RX   SERCOM5_DMAC_ID_RX

struct usart_module usart_instance;

#define MAX_RX_BUFFER_LENGTH 1
char * rx_buffer;

struct usart_module c;


void configure_usart(void);
void configure_usart_callbacks(void); 
void print(uint8_t * string, uint16_t size); //prints a char[] 
void println(uint8_t * string, uint16_t size); //prints a char[] with a new line
void printint(uint32_t integer); //prints an integer
void usart_write_callback(struct usart_module *const usart_module);
void usart_read_callback(struct usart_module *const usart_module);
void setBufData(char data);

#endif /* UARTSETUP_H_ */