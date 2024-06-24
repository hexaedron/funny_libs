#include "funny_time.h"

//Globals
static volatile uint64_t     _millis       = 0ULL;    // Millisecons counter
static volatile uint32_t     _btn_millis   = 0;       // Button millisecons counter
static volatile bool         _keyPressed   = false;   // Button pressed flag  
static volatile bool         _keyHeld      = false;   // Button held flag
static volatile bool         _keyRaw       = false;   // Raw key read status

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

void system_initSystick(void)
{
  NVIC_EnableIRQ(SysTicK_IRQn);

  SysTick->SR   = 0;
  SysTick->CMP  = DELAY_MS_TIME; // 1 ms
  SysTick->CNT  = 0; 
  SysTick->CTLR |= STK_CTRL_STE | STK_CTRL_STIE | STK_CTRL_STCK ;
}

void keyTick()
{
    static uint32_t keyTemp = 0;

    if(_btn_millis < BUTTON_TICK_MS) return;
    _btn_millis = 0;
    
    if (!_keyRaw)           // Button pressed
    {
        ++keyTemp;    
    }
    else                    // Button released  
    {
        if (keyTemp >= (BUTTON_HOLD_TIMEOUT_MS / BUTTON_TICK_MS))        
        {
            _keyHeld = true;        
        }
        else        
        {
            if (keyTemp > (BUTTON_DEBOUNCE_MS / BUTTON_TICK_MS)) _keyPressed = true;        
        }
        keyTemp = 0;    
    }
}

bool btnClick(void)
{
  if(_keyPressed)
  {
    _keyPressed = false;
    return true;
  }
  return false;
}

bool btnHeld(void)
{
  if(_keyHeld)
  {
    _keyHeld = false;
    return true;
  }
  return false;
}

/**
*   Systick interrupt handler. It only counts millis.
*/
extern "C" INTERRUPT_HANDLER
__attribute__((section(".srodata"))) 
void SysTick_Handler(void)
{ 
  _millis++;
  _btn_millis++;
  _keyRaw = funDigitalRead(PC6);
  SysTick->SR = 0;
  SysTick->CMP += DELAY_MS_TIME;
}