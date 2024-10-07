#include "vk16k33.h"

void vk16k33::refresh()
{
    I2C_startWrite(this->i2c_address);
        for(uint8_t i = 0; i < sizeof(this->data); i++)
            I2C_write(this->data[i]);
    I2C_stop();
}

void vk16k33::digit( uint8_t digit, uint8_t idx, bool dot)
{
    data[idx * 2 + 1] = digits[digit] & 0xFF;
    data[idx * 2 + 2] = digits[digit] >> 8;
    if(dot) 
    {
        data[idx * 2 + 2] |= (VK16K33_SEGMENT_DOT >> 8);
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
    this->brightness = 0;
    
    I2C_startWrite(this->i2c_address);
	    I2C_write(VK16K33_CMD_INIT);
	I2C_stop();

	I2C_startWrite(this->i2c_address);
	    I2C_write(VK16K33_CMD_SETUP | VK16K33_CMD_DISPLAY_ON);
	I2C_stop();

	I2C_startWrite(this->i2c_address);
	    I2C_write(VK16K33_CMD_BRIGHTNESS);
	I2C_stop();
}

void vk16k33::setBrightness(uint8_t br = 0)
{
    if(br > 0xF) br = 0xF;
    this->brightness = br;

    I2C_startWrite(this->i2c_address);
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

    I2C_startWrite(this->i2c_address);
	    I2C_write(VK16K33_CMD_SETUP | VK16K33_CMD_DISPLAY_ON | blink);
	I2C_stop();
}

void vk16k33::decBrightness(void)
{
    if(this->brightness != 0)
    {
        this->brightness--;
        this->setBrightness(this->brightness);
    }
}

void vk16k33::incBrightness(void)
{
    if(this->brightness != 16)
    {
        this->brightness++;
        this->setBrightness(this->brightness);
    }
}

uint8_t vk16k33::getBrightness(void)
{
    return this->brightness;
}