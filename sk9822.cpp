#include "sk9822.h"

void sk9822Out(uint32_t* sk9822_array, uint16_t leds)
{
	uint16_t i;
	volatile uint8_t* sk9822_array_8 = (uint8_t*)sk9822_array;

	SPI_begin_8();

	// Start frame
	SPI_write_8(0x00);SPI_wait_transmit_finished();
	SPI_write_8(0x00);SPI_wait_transmit_finished();
	SPI_write_8(0x00);SPI_wait_transmit_finished();
	SPI_write_8(0x00);SPI_wait_transmit_finished();


	for (i = 0; i < (leds << 2); i++)
	{
		SPI_write_8(sk9822_array_8[i]);
		SPI_wait_transmit_finished();
	}


	// Reset frame - Only needed for SK9822, has no effect on APA102
	SPI_write_8(0x00);SPI_wait_transmit_finished();
	SPI_write_8(0x00);SPI_wait_transmit_finished();
	SPI_write_8(0x00);SPI_wait_transmit_finished();
	SPI_write_8(0x00);SPI_wait_transmit_finished();   
	for (i = 0; i < leds; i += 16)
	{
	  SPI_write_8(0x00);  // 8 more clock cycles
	  SPI_wait_transmit_finished();
	}

	SPI_end();
}