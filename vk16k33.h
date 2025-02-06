#pragma once
#include "ch32fun.h"
#include "i2c_dma.h"

#include <stdbool.h>
#include <cstdio>
#include <cstring>

#define VK16K33_DEFAULT_I2C_ADDRESS 0x70

#define VK16K33_BLINK_OFF   0x00
#define VK16K33_BLINK_1HZ   0x02
#define VK16K33_BLINK_2HZ   0x04
#define VK16K33_BLINK_0HZ5  0x06

#define VK16K33_CMD_BRIGHTNESS  0xE0
#define VK16K33_CMD_INIT        0x21
#define VK16K33_CMD_SETUP       0x80
#define VK16K33_CMD_DISPLAY_OFF 0x00
#define VK16K33_CMD_DISPLAY_ON  0x01

// прямые
#define VK16K33_SEGMENT_UP       0x0001 // верх
#define VK16K33_SEGMENT_R_UP     0x0002 // прав верх
#define VK16K33_SEGMENT_R_DOWN   0x0004 // прав низ
#define VK16K33_SEGMENT_DOWN     0x0008 // низ
#define VK16K33_SEGMENT_L_DOWN   0x0010 // лев низ
#define VK16K33_SEGMENT_L_UP     0x0020 // лев верх
#define VK16K33_SEGMENT_L_CENTER 0x0040 // лев центр
#define VK16K33_SEGMENT_R_CENTER 0x0080 // прав центр
    // косые
#define VK16K33_SEGMENT_SLASH_L_UO        0x0100, // лев верх
#define VK16K33_SEGMENT_SLASH_CENTER_UP   0x0200, // центр верх
#define VK16K33_SEGMENT_SLASH_R_UP        0x0400, // прав верх
#define VK16K33_SEGMENT_SLASH_L_DOWN      0x0800, // лев низ
#define VK16K33_SEGMENT_SLASH_CENTER_DOWN 0x1000, // центр низ
#define VK16K33_SEGMENT_SLASH_R_DOWN      0x2000, // прав низ
#define VK16K33_SEGMENT_DOT               0x4000 // точка

const uint16_t digits[] = 
{
  0x003F, // 0
  0x0406, // 1
  0x088B, // 2
  0x008F, // 3
  0x00E6, // 4
  0x00ED, // 5
  0x00FD, // 6
  0x1401, // 7
  0x00FF, // 8
  0x00EF, // 9
  0x00E3, // deg
  0x2DE4, // %
  0     , // SPACE
  0x0700, // Arrow down
  0x3800  // Arrow up 
};

class vk16k33
{
private:
    uint8_t data[9] = {0};
    uint8_t i2c_address = VK16K33_DEFAULT_I2C_ADDRESS;
    uint8_t brightness;
public:
    void setI2CAddress(uint8_t);    
    void init();
    void refresh();
    void digit( uint8_t, uint8_t, bool dot = false);
    void clear();
    void setBrightness(uint8_t);
    void setBlink(uint8_t);
    void incBrightness(void);
    void decBrightness(void);
    uint8_t getBrightness(void);
};



