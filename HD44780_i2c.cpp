#include "HD44780_i2c.h"
#include <cstdlib>
#include <cstdio>

HD44780_i2c::HD44780_i2c(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
{
    this->_Addr = lcd_Addr;
    this->_cols = lcd_cols;
    this->_rows = lcd_rows;
    this->_backlightval = LCD_NOBACKLIGHT;
}

bool HD44780_i2c::detected()
{
	return i2c_ping(_Addr);
}

void HD44780_i2c::init()
{
    this->_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    this->begin(this->_cols, this->_rows, LCD_5x8DOTS);
}

void HD44780_i2c::begin(uint8_t cols, uint8_t lines, uint8_t dotsize)
{
    if (lines > 1) 
    {
		this->_displayfunction |= LCD_2LINE;
	}
	this->_numlines = lines;

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != 0) && (lines == 1)) 
    {
		this->_displayfunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	Delay_Ms(50); 
  
	// Now we pull both RS and R/W low to begin commands
	this->expanderWrite(this->_backlightval);	// reset expanderand turn backlight off (Bit 8 =1)

  	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46
	
	  // we start in 8bit mode, try to set 4 bit mode
   this->write4bits(0x03 << 4);
   Delay_Us(4500); // wait min 4.1ms
   
   // second try
   this->write4bits(0x03 << 4);
   Delay_Us(4500); // wait min 4.1ms
   
   // third go!
   this->write4bits(0x03 << 4); 
   Delay_Us(150);
   
   // finally, set to 4-bit interface
   this->write4bits(0x02 << 4); 


	// set # lines, font size, etc.
	this->command(LCD_FUNCTIONSET | this->_displayfunction);  
	
	// turn the display on with no cursor or blinking default
	this->_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	this->display();
	
	// clear it off
	this->clear();
	
	// Initialize to default text direction (for roman languages)
	this->_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	
	// set the entry mode
	this->command(LCD_ENTRYMODESET | _displaymode);
	
	this->home();
}

void HD44780_i2c::command(uint8_t value)
{
    this->send(value, 0);
}

void HD44780_i2c::send(uint8_t value, uint8_t mode) 
{
	uint8_t highnib=value&0xf0;
	uint8_t lownib=(value<<4)&0xf0;
    this->write4bits((highnib)|mode);
	this->write4bits((lownib)|mode); 
}

void HD44780_i2c::write4bits(uint8_t value) 
{
	this->expanderWrite(value);
	this->pulseEnable(value);
}

void HD44780_i2c::expanderWrite(uint8_t _data)
{     
    I2C_startWrite(this->_Addr);
        I2C_write((_data) | this->_backlightval);
	I2C_stop();  
}

void HD44780_i2c::pulseEnable(uint8_t _data)
{
	this->expanderWrite(_data | En);	// En high
	Delay_Us(1);		// enable pulse must be >450ns
	
	this->expanderWrite(_data & ~En);	// En low
	Delay_Us(50);		// commands need > 37us to settle
} 

void HD44780_i2c::display()
{
	this->_displaycontrol |= LCD_DISPLAYON;
	this->command(LCD_DISPLAYCONTROL | this->_displaycontrol);
}

void HD44780_i2c::clear()
{
	this->command(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
	Delay_Us(2000);  // this command takes a long time!
    this->setCursor(0,0);
}

void HD44780_i2c::setCursor(uint8_t col, uint8_t row)
{
	const uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if ( row > this->_numlines ) {
		row = this->_numlines-1;    // we count rows starting w/0
	}
	this->command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void HD44780_i2c::home()
{
	this->command(LCD_RETURNHOME);  // set cursor position to zero
	Delay_Ms(2);  // this command takes a long time!
}

// Turn the (optional) backlight off/on
void HD44780_i2c::noBacklight(void) 
{
	this->_backlightval=LCD_NOBACKLIGHT;
	this->expanderWrite(0);
}

void HD44780_i2c::backlight(void) 
{
	this->_backlightval=LCD_BACKLIGHT;
	this->expanderWrite(0);
}

void HD44780_i2c::printChar(char c)
{
    this->send(c, 1);
}

void HD44780_i2c::createChar(uint8_t location, const uint8_t charmap[]) 
{
	location &= 0x7; // we only have 8 locations 0-7
	this->command(LCD_SETCGRAMADDR | (location << 3));
	for (int i=0; i<8; i++) 
	{
		this->send(charmap[i], 1);
	}
}


// Turns the underline cursor on/off
void HD44780_i2c::noCursor() {
	_displaycontrol &= ~LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void HD44780_i2c::cursor() {
	_displaycontrol |= LCD_CURSORON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void HD44780_i2c::noBlink() {
	_displaycontrol &= ~LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void HD44780_i2c::blink() {
	_displaycontrol |= LCD_BLINKON;
	command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void HD44780_i2c::scrollDisplayLeft(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void HD44780_i2c::scrollDisplayRight(void) {
	command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void HD44780_i2c::leftToRight(void) {
	_displaymode |= LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void HD44780_i2c::rightToLeft(void) {
	_displaymode &= ~LCD_ENTRYLEFT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void HD44780_i2c::autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void HD44780_i2c::noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	command(LCD_ENTRYMODESET | _displaymode);
}

void HD44780_i2c::print(char* str)
{
	uint8_t pos = 0;
	while (str[pos] != '\0')
	{
		this->printChar(str[pos]);
		pos++;
	}
	
}

void HD44780_i2c::print(uint32_t val, uint32_t radix)
{
	char buf[11];
	itoa(val, buf, radix);
	this->print(buf);
}

void HD44780_i2c::print( int32_t val, uint32_t radix)
{
	char buf[12];
	utoa(val, buf, radix);
	this->print(buf);
}

void HD44780_i2c::print( float val)
{
	char buf[12]; //size of the number
	snprintf(buf, 11, "%g", val);
	this->print(buf);
}
