#pragma once
#include <ch32fun.h>
#include <cstdint>

#include "funny_defs.h"

// Arduino-like millis()
uint64_t millis(void);

// Arduino-like millis(). Does not require interrupt masking
uint32_t millis32(void);

void system_initSystick(uint32_t period = DELAY_MS_TIME /* 1 ms */);