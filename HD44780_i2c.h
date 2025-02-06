#ifndef HD44780_i2c_h
#define HD44780_i2c_h

#include <ch32fun.h>
#include <cstdint>
#include "i2c_dma.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

class HD44780_i2c
{
    public:
        HD44780_i2c(uint8_t lcd_Addr = 0x27, uint8_t lcd_cols = 20, uint8_t lcd_rows = 4);
        void init();
        void clear();
        void home();
        void display();
        void setCursor(uint8_t, uint8_t);
        void noBacklight();
        void backlight();
        void printChar(char);
        void createChar(uint8_t, const uint8_t[]);
        void noBlink();
        void blink();
        void noCursor();
        void cursor();
        void scrollDisplayLeft();
        void scrollDisplayRight();
        void printLeft();
        void printRight();
        void leftToRight();
        void rightToLeft();
        void shiftIncrement();
        void shiftDecrement();
        void autoscroll();
        void noAutoscroll();
        void print(char*);
        void print(uint32_t val, uint32_t radix = 10);
        void print( int32_t val, uint32_t radix = 10);
        void print(uint64_t val, uint32_t radix = 10);
        void print( int64_t val, uint32_t radix = 10);
        void print( float); //Better avoid this.

    private:
        void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS );
        void write4bits(uint8_t);
        void expanderWrite(uint8_t);
        void command(uint8_t);
        void send(uint8_t, uint8_t);
        void pulseEnable(uint8_t);
        uint8_t _Addr;
        uint8_t _displayfunction;
        uint8_t _displaycontrol;
        uint8_t _displaymode;
        uint8_t _numlines;
        uint8_t _cols;
        uint8_t _rows;
        uint8_t _backlightval;
};

// Cyrillic glyphs
namespace cyrillicGlyphs
{
    const uint8_t Be[]      = {0x1f, 0x11, 0x10, 0x1e, 0x11, 0x11, 0x1e, 0x00};
    const uint8_t Ghe[]     = {0x1f, 0x11, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00};
    const uint8_t De[]      = {0x0f, 0x05, 0x05, 0x05, 0x09, 0x11, 0x1f, 0x11};
    const uint8_t Zhe[]     = {0x15, 0x15, 0x15, 0x0e, 0x15, 0x15, 0x15, 0x00};
    const uint8_t Ze[]      = {0x1e, 0x01, 0x01, 0x0e, 0x01, 0x01, 0x1e, 0x00};
    const uint8_t I[]       = {0x11, 0x11, 0x13, 0x15, 0x19, 0x11, 0x11, 0x00};
    const uint8_t Ikr[]     = {0x0a, 0x04, 0x11, 0x13, 0x15, 0x19, 0x11, 0x00};
    const uint8_t L[]       = {0x0f, 0x05, 0x05, 0x05, 0x05, 0x15, 0x09, 0x00};
    const uint8_t P[]       = {0x1f, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x00};
    const uint8_t F[]       = {0x04, 0x0e, 0x15, 0x15, 0x15, 0x0e, 0x04, 0x00};
    const uint8_t Tse[]     = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1f, 0x01};
    const uint8_t Tsche[]   = {0x11, 0x11, 0x11, 0x0f, 0x01, 0x01, 0x01, 0x00};
    const uint8_t Sha[]     = {0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x1f, 0x00};
    const uint8_t Scha[]    = {0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x1f, 0x01};
    const uint8_t Tv_znak[] = {0x18, 0x08, 0x08, 0x0e, 0x09, 0x09, 0x0e, 0x00};
    const uint8_t Oui[]     = {0x11, 0x11, 0x11, 0x19, 0x15, 0x15, 0x19, 0x00};
    const uint8_t Mg_znak[] = {0x08, 0x08, 0x08, 0x0e, 0x09, 0x09, 0x0e, 0x00};
    const uint8_t E[]       = {0x0e, 0x11, 0x01, 0x07, 0x01, 0x11, 0x0e, 0x00};
    const uint8_t You[]     = {0x12, 0x15, 0x15, 0x1d, 0x15, 0x15, 0x12, 0x00};
    const uint8_t Ya[]      = {0x0f, 0x11, 0x11, 0x0f, 0x05, 0x09, 0x11, 0x00};
    const uint8_t Yo[]      = {0x0a, 0x00, 0x1f, 0x10, 0x1e, 0x10, 0x1f, 0x00};
    const uint8_t Y[]       = {0x11, 0x11, 0x11, 0x0a, 0x04, 0x08, 0x10, 0x00};

    const uint8_t be[]      = {0x03, 0x0c, 0x10, 0x1e, 0x11, 0x11, 0x0e, 0x00};
    const uint8_t ghe[]     = {0x00, 0x00, 0x1e, 0x12, 0x10, 0x10, 0x10, 0x00};
    const uint8_t de[]      = {0x00, 0x00, 0x0f, 0x05, 0x09, 0x11, 0x1f, 0x11};
    const uint8_t zhe[]     = {0x00, 0x00, 0x15, 0x15, 0x0e, 0x15, 0x15, 0x00};
    const uint8_t ze[]      = {0x00, 0x00, 0x1c, 0x02, 0x0c, 0x02, 0x1c, 0x00};
    const uint8_t i[]       = {0x00, 0x00, 0x11, 0x13, 0x15, 0x19, 0x11, 0x00};
    const uint8_t ikr[]     = {0x00, 0x0a, 0x04, 0x11, 0x13, 0x15, 0x19, 0x00};
    const uint8_t l[]       = {0x00, 0x00, 0x0f, 0x05, 0x05, 0x15, 0x09, 0x00};
    const uint8_t p[]       = {0x00, 0x00, 0x1f, 0x11, 0x11, 0x11, 0x11, 0x00};
    const uint8_t f[]       = {0x00, 0x04, 0x04, 0x0e, 0x15, 0x15, 0x0e, 0x04};
    const uint8_t tse[]     = {0x00, 0x00, 0x12, 0x12, 0x12, 0x12, 0x1f, 0x01};
    const uint8_t tsche[]   = {0x00, 0x00, 0x11, 0x11, 0x0f, 0x01, 0x01, 0x00};
    const uint8_t sha[]     = {0x00, 0x00, 0x15, 0x15, 0x15, 0x15, 0x1f, 0x00};
    const uint8_t scha[]    = {0x00, 0x00, 0x15, 0x15, 0x15, 0x15, 0x1f, 0x01};
    const uint8_t tv_znak[] = {0x00, 0x00, 0x18, 0x08, 0x0e, 0x09, 0x0e, 0x00};
    const uint8_t oui[]     = {0x00, 0x00, 0x11, 0x11, 0x1d, 0x13, 0x1d, 0x00};
    const uint8_t mg_znak[] = {0x00, 0x00, 0x10, 0x10, 0x1c, 0x12, 0x1c, 0x00};
    const uint8_t e[]       = {0x00, 0x00, 0x0e, 0x11, 0x07, 0x11, 0x0e, 0x00};
    const uint8_t you[]     = {0x00, 0x00, 0x12, 0x15, 0x1d, 0x15, 0x12, 0x00};
    const uint8_t ya[]      = {0x00, 0x00, 0x0f, 0x11, 0x0f, 0x05, 0x09, 0x00};
    const uint8_t yo[]      = {0x0a, 0x00, 0x0e, 0x11, 0x1f, 0x10, 0x0e, 0x00};
    const uint8_t t[]       = {0x00, 0x00, 0x1f, 0x04, 0x04, 0x04, 0x04, 0x00};
};

#endif