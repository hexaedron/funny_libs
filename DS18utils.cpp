#include "DS18utils.h"

// Based on a perfect AlexGyver library:
// https://github.com/GyverLibs/GyverDS18

namespace gds {

uint8_t calcResolution(uint8_t* buf) {
    return ((buf[4] >> 5) & 0xff) + 9;
}

int16_t calcTemp(uint8_t* buf) {
    return buf[0] | (buf[1] << 8);
}

void copyAddress(const uint64_t& address, uint8_t* buf) {
    buf64 addr{address};
    for (uint8_t i = 0; i < 8; i++) {
        buf[i] = addr.u8[i];
    }
}

// https://stackoverflow.com/questions/15053776/how-do-you-disable-the-unused-variable-warnings-coming-out-of-gcc-in-3rd-party-c
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static char _getChar(uint8_t b) {
    return (char)(b + ((b > 9) ? 55 : '0'));
}
#pragma GCC diagnostic pop

}  // namespace gds
