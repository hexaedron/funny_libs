#pragma once
#include <ch32fun.h>
#include <cstring>

//https://cpldcpu.com/2016/12/13/sk9822-a-clone-of-the-apa102/
//https://github.com/cpldcpu/light_ws2812/blob/master/light_apa102_AVR/Light_apa102/light_apa102.c

#define CH32V003_SPI_SPEED_HZ 1000000
#define CH32V003_SPI_CLK_MODE_POL0_PHA0	
#define CH32V003_SPI_DIRECTION_1LINE_TX
#define CH32V003_SPI_NSS_SOFTWARE_ANY_MANUAL
#include <ch32v003_SPI.h>

#define SK9822_VALUE(brigtness, R, G, B ) ( ((uint32_t)( 0xE0UL + (brigtness)) ) + ((uint32_t)(B) << 8) + ((uint32_t)(G) << 16) + ((uint32_t)(R) << 24) )

void sk9822Out(uint32_t* sk9822_array, uint16_t leds);