#pragma once
#include <ch32fun.h>

namespace gds {

struct RAM {
    uint8_t t_lsb;
    uint8_t t_msb;
    uint8_t th;
    uint8_t tl;
    uint8_t cfg;

    int16_t getTemp() {
        return t_lsb | (t_msb << 8);
    }
    uint8_t getRes() {
        return ((cfg >> 5) & 0xff) + 9;
    }
};

class Addr {
   public:
    uint64_t addr = 0;

    Addr() {}
    Addr(const uint64_t addr) : addr(addr) {}

    operator uint64_t() {
        return addr;
    }

    operator bool() {
        return addr;
    }

    uint8_t& operator[](uint8_t i) {
        return ((uint8_t*)&addr)[i];
    }

    void copyTo(uint8_t* buf) {
        memcpy(buf, &addr, 8);
    }

    void printTo(char* pr, bool addZero = true) {
        uint8_t* p = (uint8_t*)&addr;
        uint32_t j = 0;
        for (int32_t i = 7; i >= 0; i--) 
        {
            pr[j] = _getChar(p[i] >> 4);
            pr[j+1] = _getChar(p[i] & 0xF);
            j += 2;
        }
        if(addZero) pr[j] = '\0';
    }

   private:
    char _getChar(uint8_t b) {
        return b + ((b > 9) ? 55 : '0');
    }
};

}// namespace gds