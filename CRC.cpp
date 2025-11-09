#include <ch32fun.h>

static uint32_t ReverseUint32(uint32_t x)
{
    uint32_t mask = 0b11111111111111110000000000000000;
    x = (x & mask) >> 16 | (x & ~mask) << 16;
    mask = 0b11111111000000001111111100000000;
    x = (x & mask) >> 8 | (x & ~mask) << 8;
    mask = 0b11110000111100001111000011110000;
    x = (x & mask) >> 4 | (x & ~mask) << 4;
    mask = 0b11001100110011001100110011001100;
    x = (x & mask) >> 2 | (x & ~mask) << 2;
    mask = 0b10101010101010101010101010101010;
    x = (x & mask) >> 1 | (x & ~mask) << 1;
    return x;
}

void initCRC()
{
    RCC->AHBPCENR |= RCC_AHBPeriph_CRC;
    CRC->CTLR = CRC_CTLR_RESET;
}

void resetCRC()
{
    CRC->CTLR = CRC_CTLR_RESET;
}

void calcCRC(uint32_t *data, size_t dataSize)
{
    for(size_t i = 0; i < dataSize; i++)
    {
        CRC->DATAR = ReverseUint32(data[i]);
    }
}

uint32_t getCRC(void)
{
    return ~ReverseUint32(CRC->DATAR);
}

