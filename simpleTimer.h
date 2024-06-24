#pragma once
#include <cstdint>

uint64_t millis(void);
uint32_t millis32(void);

class simpleTimer
{
  public:
    simpleTimer();
    simpleTimer (uint64_t);
    simpleTimer (uint32_t);
    simpleTimer (int32_t);
    void setPrd(uint32_t);
    void start_int();
    void stop();
    bool ready();

  private:
    uint64_t tmr;
    uint64_t prd;
};

class simpleTimer32
{
  public:
    simpleTimer32();
    simpleTimer32 (uint32_t);
    simpleTimer32 (int32_t);
    void setPrd(uint32_t);
    void start_int();
    void stop();
    bool ready();

  private:
    uint32_t tmr;
    uint32_t prd;
};