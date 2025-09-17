#include "funny_time.h"

//Globals
volatile uint64_t     _millis       = 0ULL;    // Millisecons counter

// Arduino-like millis()
uint64_t millis(void)
{
  uint64_t tmp;

  // critical section
  NVIC_DisableIRQ(SysTicK_IRQn);
  {
    tmp = _millis;
  }
  NVIC_EnableIRQ(SysTicK_IRQn);

  return tmp;
}

// Arduino-like millis(). Does not require interrupt masking
uint32_t millis32(void)
{
  return _millis;
}

void system_initSystick(uint32_t period)
{
  SysTick->SR   = 0;
  SysTick->CMP  = period; // 1 ms
  SysTick->CNT  = 0; 
  SysTick->CTLR |= STK_CTRL_STE | STK_CTRL_STIE | STK_CTRL_STCK ;

  NVIC_SetPriority(SysTicK_IRQn, 1);
  NVIC_EnableIRQ(SysTicK_IRQn);
}