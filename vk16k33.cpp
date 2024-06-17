#include "vk16k33.h"

void vk16k33::refresh()
{
    I2C_start(0x70);
    this->data[0] = 0;
	I2C_writeBuffer(data, sizeof(data));
    I2C_stop();
}

void vk16k33::digit( uint8_t digit, uint8_t idx, bool dot)
{
    this->data[idx * 2 + 1] = digits[digit] & 0xFF;
    this->data[idx * 2 + 2] = digits[digit] >> 8;
    if(dot) 
    {
        this->data[idx * 2 + 2] |= (VK16K33_SEGMENT_DOT >> 8);
    }
}

void vk16k33::clear()
{
    memset(data, 0, sizeof(data));
}

void vk16k33::setI2CAddress(uint8_t newAddr)
{
    this->i2c_address = newAddr;
}

void vk16k33::init()
{
    I2C_start(this->i2c_address);
	    I2C_write(VK16K33_CMD_INIT);
	I2C_stop();

	I2C_start(this->i2c_address);
	    I2C_write(VK16K33_CMD_SETUP | VK16K33_CMD_DISPLAY_ON);
	I2C_stop();

	I2C_start(this->i2c_address);
	    I2C_write(VK16K33_CMD_BRIGHTNESS);
	I2C_stop();
}

void vk16k33::setBrightness(uint8_t br = 0)
{
    if(br > 0xF) br = 0xF;

    I2C_start(this->i2c_address);
	    I2C_write( VK16K33_CMD_BRIGHTNESS | br );
	I2C_stop();
}

void vk16k33::setBlink(uint8_t blink)
{
    if( (blink != VK16K33_BLINK_OFF) && 
        (blink != VK16K33_BLINK_1HZ) && 
        (blink != VK16K33_BLINK_2HZ) && 
        (blink != VK16K33_BLINK_0HZ5) )
    {
        blink = VK16K33_BLINK_OFF;
    }

    I2C_start(this->i2c_address);
	    I2C_write(VK16K33_CMD_SETUP | VK16K33_CMD_DISPLAY_ON | blink);
	I2C_stop();
}
