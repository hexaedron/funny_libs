#ifndef AHT20_H
#define AHT20_H

#include <ch32fun.h>
#include "i2c_dma.h"

class AHT20{
  private:
    uint8_t _deviceAddress;

    enum REG{
      DEFAULT_ADDRESS = 0x38,
      CMD_CALIBRATE = 0xE1,     ///< Calibration command
      CMD_INITIALIZE = 0xBE,     ///< Calibration command
      CMD_TRIGGER = 0xAC,       ///< Trigger reading command
      CMD_SOFTRESET = 0xBA     ///< Soft reset command
    };

    enum bits {
      BIT_CALIBRATED = 3,
      BIT_BUSY = 7
    };

    struct{
        uint32_t humidity;
        uint32_t temperature;
        uint8_t crc;
    } _sensorData;

    bool getStatus(uint8_t bit);       //Returns the status byte
    bool initialize();         //Initialize for taking measurement
    bool softReset();          //Restart the sensor system without turning power off and on

  public:
    AHT20(const uint8_t deviceAddress = DEFAULT_ADDRESS);//Sets the address of the device
    bool begin();                  

    bool triggerMeasurement(); //Trigger the AHT20 to take a measurement
    void readData();           //Read and parse the 6 bytes of data into raw humidity and temp
    bool isMeasuring() { return getStatus(BIT_BUSY); };
    int getTemperature(); //Goes through the measurement sequence and returns temperature in degrees celcius
    int getHumidity();    //Goes through the measurement sequence and returns humidity in % RH
    bool detected() {return i2c_ping(this->_deviceAddress); };
};
#endif
