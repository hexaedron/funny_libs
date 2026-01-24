/****************************************************************
*
 ***************************************************************/
#include "AHT20.h"

AHT20::AHT20(const uint8_t deviceAddress){
  _deviceAddress = deviceAddress;
}

bool AHT20::begin()
{
    softReset();
    Delay_Ms(10);
    initialize();
    Delay_Ms(10);
    triggerMeasurement();
    Delay_Ms(80); //Wait for measurement to complete
    uint8_t counter = 0;
    while (getStatus(BIT_BUSY)){
        Delay_Ms(1);
        if (counter++ > 100) return false;
    }
    if (!getStatus(BIT_CALIBRATED)) return false;

  return true;
}

/*------------------------ Measurement Helpers ---------------------------*/

bool AHT20::getStatus(uint8_t bit)
{
  I2C_startRead(this->_deviceAddress);
    uint8_t res = I2C_read(0) & (1 << bit);
  I2C_stop();
    return res;
}

bool AHT20::initialize()
{
    I2C_startWrite(this->_deviceAddress);
      I2C_write(CMD_INITIALIZE);
      I2C_write((uint8_t)0x08);
      I2C_write((uint8_t)0x00);
    I2C_stop();
    return true;
}

bool AHT20::triggerMeasurement()
{
    I2C_startWrite(this->_deviceAddress);
      I2C_write(CMD_TRIGGER);
      I2C_write((uint8_t)0x33);
      I2C_write((uint8_t)0x00);
    I2C_stop();
    return true;
}

void AHT20::readData()
{
    _sensorData.temperature = 0;
    _sensorData.humidity = 0;
    uint8_t buf[7];

    I2C_startRead(this->_deviceAddress);
        I2C_readBuffer(buf, 7); 
    I2C_stop();

    uint32_t incoming = 0;
    incoming = ((uint32_t)buf[1] << 16);
    incoming |= ((uint32_t)buf[2] << 8);
    uint8_t midByte = buf[3];

    incoming |= midByte;
    _sensorData.humidity = incoming >> 4; // 20bits

    _sensorData.temperature = ((uint32_t)midByte << 16);
    _sensorData.temperature |= ((uint32_t)buf[4] << 8);
    _sensorData.temperature |= (uint32_t)buf[5];

    _sensorData.crc = buf[7]; // last byte
    //Need to get rid of data in bits > 20
    _sensorData.temperature = _sensorData.temperature & ~(0xFFF00000);

}

bool AHT20::softReset()
{
    I2C_startWrite(this->_deviceAddress);
      I2C_write(CMD_SOFTRESET);
    I2C_stop();
    return true;
}

int AHT20::getTemperature()
{
  //From datasheet pg 8
  int64_t t10 = (((int64_t)_sensorData.temperature * 2000LL) >> 20) - 500LL;
  return (int32_t)(t10);
}

int AHT20::getHumidity()
{
  //From datasheet pg 8
  int hum10 = ((int)_sensorData.humidity * 1000) >> 20;
  return (int)hum10;
}
