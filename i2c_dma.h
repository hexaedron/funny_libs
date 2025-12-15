// ===================================================================================
// Basic I2C Master Functions with DMA for TX for CH32V003                    * v1.0 *
// ===================================================================================
//
// Functions available:
// --------------------
// I2C_init()               Init I2C with defined clock rate (see below)
// I2C_start(addr)          I2C start transmission, addr must contain R/W bit
// I2C_write(b)             I2C transmit one data byte via I2C
// I2C_read(ack)            I2C receive one data byte (set ack=0 for last byte)
// I2C_stop()               I2C stop transmission
//
// I2C_writeBuffer(buf,len) Send buffer (*buf) with length (len) via I2C/DMA and stop
//
// I2C pin mapping (set below in I2C parameters):
// ----------------------------------------------
// I2C_MAP    0     1     2
// SDA-pin   PC1   PD0   PC6
// SCL-pin   PC2   PD1   PC5
//
// External pull-up resistors (4k7 - 10k) are mandatory!
// 2023 by Stefan Wagner:   https://github.com/wagiminator

#pragma once

#include "ch32fun.h"
#include "funny_defs.h"

#define F_CPU FUNCONF_SYSTEM_CORE_CLOCK

// I2C Functions
void I2C_init(const uint32_t clkrate = 100000, const uint32_t SCLpin = PB6, const uint32_t SDApin = PB7);              // I2C init function
bool I2C_start(uint8_t addr, int32_t timeout = 10000);     // I2C start transmission, addr must contain R/W bit
void I2C_stop(void);              // I2C stop transmission
void I2C_write(uint8_t data, int32_t timeout = 10000);     // I2C transmit one data byte via I2C
uint8_t I2C_read(uint8_t ack, int32_t timeout = 10000);    // I2C receive one data byte from the slave
#define I2C_startRead(addr)  I2C_start(((addr) << 1) | (uint8_t)1)
#define I2C_startWrite(addr) I2C_start(((addr) << 1) | (uint8_t)0)

void I2C_writeBuffer(uint8_t* buf, uint16_t len);
void I2C_readBuffer(uint8_t* buf, uint16_t len);

uint8_t  I2C_readReg8(uint8_t addr, uint8_t reg);
void     I2C_writeReg8(uint8_t addr, uint8_t reg, uint8_t value);
void     I2C_readMulti(uint8_t addr, uint8_t reg, uint8_t * dst, uint8_t count);

#define I2C_busy()                (I2C1->STAR2 & I2C_STAR2_BUSY)

typedef enum 
{
	I2C_OK	  = 0,   // No Error. All OK
	I2C_ERR_BERR,	 // Bus Error
	I2C_ERR_NACK,	 // ACK Bit failed
	I2C_ERR_ARLO,	 // Arbitration Lost
	I2C_ERR_OVR,	 // Overun/underrun condition
	I2C_ERR_BUSY,	 // Bus was busy and timed out
} i2c_err_t;

bool i2c_ping(const uint8_t addr);

