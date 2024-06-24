#include "simpleTimer.h"
    
simpleTimer::simpleTimer () 
{

}

simpleTimer::simpleTimer (uint64_t nprd) 
{
  prd = nprd;
  start_int();
}

simpleTimer::simpleTimer (uint32_t nprd) 
{
  prd = (uint64_t)nprd;
  start_int();
}

simpleTimer::simpleTimer (int32_t nprd) 
{
  prd = (uint64_t)nprd;
  start_int();
}

void simpleTimer::setPrd(uint32_t nprd)
{
  prd = (uint64_t)nprd;
}

void simpleTimer::start_int() 
{
  tmr = millis();
  if (tmr == 0ULL) tmr = 1ULL;
}

void simpleTimer::stop() 
{
  tmr = 0ULL;
}

bool simpleTimer::ready() 
{
  if ( (tmr != 0ULL) && ( (millis() - tmr) >= prd ) )
  {
    start_int();
    return true;
  }

  return false;
}

simpleTimer32::simpleTimer32 () 
{

}

simpleTimer32::simpleTimer32 (uint32_t nprd) 
{
  prd = nprd;
  start_int();
}

simpleTimer32::simpleTimer32 (int32_t nprd) 
{
  prd = nprd;
  start_int();
}

void simpleTimer32::setPrd(uint32_t nprd)
{
  prd = nprd;
}

void simpleTimer32::start_int() 
{
  tmr = millis();
  if (tmr == 0UL) tmr = 1UL;
}

void simpleTimer32::stop() 
{
  tmr = 0UL;
}

bool simpleTimer32::ready() 
{
  if ( (tmr != 0UL) && ( (millis32() - tmr) >= prd ) )
  {
    start_int();
    return true;
  }

  return false;
}