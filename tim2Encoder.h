#pragma once
#include "ch32v003fun.h"

class tim2Encoder
{
    public:
        //tim2Encoder() {};
        tim2Encoder(const uint32_t);
        void init(const uint32_t);
        int32_t getDelta();
    
    private:
        uint16_t count;
        uint16_t last_count;
};
