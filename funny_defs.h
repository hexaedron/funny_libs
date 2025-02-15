#pragma once

#define STK_CTRL_STRE 0b0000000000000000000000000001000
#define STK_CTRL_STIE 0b0000000000000000000000000000010
#define STK_CTRL_STE  0b0000000000000000000000000000001
#define STK_CTRL_SWIE 0b1000000000000000000000000000000
#define STK_CTRL_STCK 0b0000000000000000000000000000100

#define HW_PORT_NUM(pin) ( (pin & 0xFFFFFFF0) >> 4 )
#define HW_PIN_NUM(pin)  (  pin & 0xF )

// Arduino-like bit macros
#define bit(n)                    (1 << (n))
#define bitSet(value, n)          ((value) |= bit(n))
#define bitClear(value, n)        ((value) &= ~bit(n))
#define bitRead(value, n)         (((value) & bit(n)) !=0 )
#define bitWrite(value, n, bit)   ((bit) ? bitSet((value), (n)) : bitClear((value), (n)))

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#define map(x, in_min, in_max, out_min, out_max)  ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

#define ASCII_TO_INT(x) (x - 48)

#define FUN_INPUT_PULLUP 1
#define FUN_INPUT_PULLDOWN 0
#define funInputPullUpDown( pin, mode ) { GpioOf( pin )->OUTDR = (GpioOf( pin )->OUTDR & ~(1 << (pin & 0xf))) | (mode << (pin & 0xf)); }
