#ifndef SYSTICK_INTERRUPT_H
#define SYSTICK_INTERRUPT_H

#include <ch32fun.h>

/**
*   Systick interrupt handler. It only counts millis.
*/
extern "C" INTERRUPT_DECORATOR
__attribute__((section(".srodata"))) 
void SysTick_Handler(void)
{ 
  _millis++;
  _btn_millis++;
  _keyRaw = funDigitalRead(PC6);
  SysTick->SR = 0;
  SysTick->CMP += DELAY_MS_TIME;
}

#endif