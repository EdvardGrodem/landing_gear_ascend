#include "asf.h"
#include "UARTsetup.h"
#define SPI0_MISO_PIN PORT_PA10
#define SPI0_MOSI_PIN PORT_PA08
#define SPI0_CLK_PIN PORT_PA09
#define SLAVE0_SELECT_PIN PORT_PA11


#define SPI0_MODULE SERCOM0
#define SPI0_MUX_SETTING SPI_SIGNAL_MUX_SETTING_C
#define SPI0_PINMUX_PAD0 ((PIN_PA08C_SERCOM0_PAD0<< 16) | MUX_PA08C_SERCOM0_PAD0)
#define SPI0_PINMUX_PAD1 ((PIN_PA09C_SERCOM0_PAD1<< 16) | MUX_PA09C_SERCOM0_PAD1)
#define SPI0_PINMUX_PAD2 ((PIN_PA10C_SERCOM0_PAD2<< 16) | MUX_PA10C_SERCOM0_PAD2)

//-----------------------------------------------------------------------------------------------

#define SPI1_MISO_PIN PORT_PA18
#define SPI1_MOSI_PIN PORT_PA16
#define SPI1_CLK_PIN PORT_PA17
#define SLAVE1_SELECT_PIN PORT_PA19


#define SPI1_MODULE SERCOM1
#define SPI1_MUX_SETTING SPI_SIGNAL_MUX_SETTING_C
#define SPI1_PINMUX_PAD0 ((PIN_PA16C_SERCOM1_PAD0<< 16) | MUX_PA16C_SERCOM1_PAD0)
#define SPI1_PINMUX_PAD1 ((PIN_PA17C_SERCOM1_PAD1<< 16) | MUX_PA17C_SERCOM1_PAD1)
#define SPI1_PINMUX_PAD2 ((PIN_PA18C_SERCOM1_PAD2<< 16) | MUX_PA18C_SERCOM1_PAD2)

#define SPI_BAUDRATE 300000

struct spi_module spi0_master_instance;
struct spi_module spi1_master_instance;


void configure_spi_master(void);
void send_data(int64_t data, int ADC_index, uint64_t bytes);
void read_data(int64_t * data, int ADC_index, uint64_t bytes);
/*
void SERCOM1_Handler() {
	PORT->Group[0].OUTSET.reg = SLAVE1_SELECT_PIN;
	SERCOM1->SPI.INTFLAG.reg = SERCOM_SPI_INTENSET_TXC;

}

void SERCOM0_Handler() {
	PORT->Group[0].OUTSET.reg = SLAVE0_SELECT_PIN;
	SERCOM0->SPI.INTFLAG.reg = SERCOM_SPI_INTENSET_TXC;
}
*/
