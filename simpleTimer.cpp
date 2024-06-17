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
  if ( !(tmr == 0ULL) && ( (millis() - tmr) >= prd ) )
  {
    start_int();
    return true;
  }

  return false;
}