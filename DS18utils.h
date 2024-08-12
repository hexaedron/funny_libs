#pragma once

// Based on a perfect AlexGyver library:
// https://github.com/GyverLibs/GyverDS18

#include <ch32v003fun.h>

namespace gds {

// прочитать разрешение из внешнего буфера (5 байт)
uint8_t calcResolution(uint8_t* buf);

// прочитать температуру из внешнего буфера (5 байт)
int16_t calcTemp(uint8_t* buf);

// копировать адрес в буфер размером 8
void copyAddress(const uint64_t& address, uint8_t* buf);

union buf64 {
    uint64_t u64;
    uint8_t u8[8];
};

}  // namespace gds