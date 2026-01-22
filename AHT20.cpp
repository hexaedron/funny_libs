/****************************************************************
*
 ***************************************************************/
#include "AHT20.h"

AHT20::AHT20(const uint8_t deviceAddress){
  _deviceAddress = deviceAddress;
}

bool AHT20::begin()
{

  //Check if the calibrated bit is set. If not, init the sensor.
  if (!getStatus(BIT_CALIBRATED)){
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
  }

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

//Loads the
void AHT20::readData()
{
    _sensorData.temperature = 0;
    _sensorData.humidity = 0;

    I2C_startRead(this->_deviceAddress);
        I2C_read(true); // status byte

        uint32_t incoming = 0;
        incoming = ((uint32_t)I2C_read(true) << 16);
        incoming |= ((uint32_t)I2C_read(true) << 8);
        uint8_t midByte = I2C_read(true);

        incoming |= midByte;
        _sensorData.humidity = incoming >> 4; // 20bits

        _sensorData.temperature = ((uint32_t)midByte << 16);
        _sensorData.temperature |= ((uint32_t)I2C_read(true) << 8);
        _sensorData.temperature |= (uint32_t)I2C_read(true);

        _sensorData.crc = I2C_read(false); // last byte
        //Need to get rid of data in bits > 20
        _sensorData.temperature = _sensorData.temperature & ~(0xFFF00000);
        
    I2C_stop();
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
  return (_sensorData.temperature / 1048576) * 200 - 500;
  
  //int64_t t1000 = ((int64_t)_sensorData.temperature * 200000LL) / 1048576LL - 50000LL;
  //return (int32_t)((t1000 + 50) / 100);
}

int AHT20::getHumidity()
{

  //From datasheet pg 8
  return (_sensorData.humidity / 1048576) * 100;
}
