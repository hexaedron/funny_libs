#pragma once
#include <cstdint>

uint64_t millis(void);

class simpleTimer 
{
  public:
    simpleTimer();
    simpleTimer (uint64_t);
    simpleTimer (uint32_t);
    simpleTimer (int32_t);
    void start_int();
    void stop();
    bool ready();

  private:
      uint64_t tmr;
      uint64_t prd;
};