#include "simpleTimer.h"
    
simpleTimer64::simpleTimer64 () 
{

}

simpleTimer64::simpleTimer64 (uint64_t nprd) 
{
  prd = nprd;
  start_int();
}

simpleTimer64::simpleTimer64 (uint32_t nprd) 
{
  prd = (uint64_t)nprd;
  start_int();
}

simpleTimer64::simpleTimer64 (int32_t nprd) 
{
  prd = (uint64_t)nprd;
  start_int();
}

void simpleTimer64::setPrd(uint32_t nprd)
{
  prd = (uint64_t)nprd;
}

void simpleTimer64::start_int() 
{
  tmr = millis();
  if (tmr == 0ULL) tmr = 1ULL;
}

void simpleTimer64::stop() 
{
  tmr = 0ULL;
}

bool simpleTimer64::ready() 
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