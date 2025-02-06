#pragma once
#include <ch32fun.h>
#include <cstdint>

#include "funny_defs.h"

#define BUTTON_DEBOUNCE_MS      100
#define BUTTON_TICK_MS           10
#define BUTTON_HOLD_TIMEOUT_MS 1000 


// Arduino-like millis()
uint64_t millis(void);

// Arduino-like millis(). Does not require interrupt masking
uint32_t millis32(void);

void system_initSystick(uint32_t period = DELAY_MS_TIME /* 1 ms */);

void keyTick();
bool btnClick(void);
bool btnHeld(void);