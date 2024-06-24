#pragma once
#include <cstdint>

#include "funny_time.h"

class simpleTimer64
{
  public:
    simpleTimer64();
    simpleTimer64 (uint64_t);
    simpleTimer64 (uint32_t);
    simpleTimer64 (int32_t);
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