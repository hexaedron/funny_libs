// ===================================================================================
// Basic I2C Master Functions with DMA for TX for CH32V003                    * v1.1 *
// ===================================================================================
// 2023 by Stefan Wagner:   https://github.com/wagiminator

#include "i2c_dma.h"

// Read/write flag
uint8_t I2C_rwflag;

// Init I2C
void I2C_init(const uint32_t clkrate, const uint32_t SCLpin, const uint32_t SDApin) {
  
  // Setup GPIO pins
  // Set pin SDA and SCL to output, open-drain, 10MHz, multiplex
  RCC->APB2PCENR |= RCC_AFIOEN | RCC_IOPCEN;
  funPinMode(SCLpin, GPIO_CFGLR_OUT_10Mhz_AF_OD);
  funPinMode(SDApin, GPIO_CFGLR_OUT_10Mhz_AF_OD);

  // Setup and enable I2C
  RCC->APB1PCENR |= RCC_I2C1EN;                   // enable I2C module clock

  I2C1->CTLR2     = 8;                            // set input clock rate
  if (clkrate > 100000)                      // Fast mode ?
  {
    I2C1->CKCFGR  = (F_CPU / (3 * clkrate))   // -> set clock division factor 1:2
                  | I2C_CKCFGR_FS;                // -> enable fast mode (400kHz)
  }
  else                                           // Standard mode?
  {
    I2C1->CKCFGR  = (F_CPU / (2 * clkrate));  // -> set clock division factor 1:1
  }

  I2C1->CTLR1   |= I2C_CTLR1_PE ;                   // enable I2C

  // Setup DMA Channel 5
  RCC->AHBPCENR |= RCC_DMA1EN;                    // enable DMA module clock
  DMA1_Channel6->PADDR = (uint32_t)&I2C1->DATAR;  // peripheral address
  DMA1_Channel6->CFGR  = DMA_CFGR6_MINC            // increment memory address
                       | DMA_CFGR6_DIR             // memory to I2C
                       | DMA_CFGR6_TCIE;           // transfer complete interrupt enable
  DMA1->INTFCR         = DMA_CGIF6;               // clear interrupt flags
  NVIC_EnableIRQ(DMA1_Channel6_IRQn);             // enable the DMA IRQ
}

// Start I2C transmission (addr must contain R/W bit)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
void I2C_start(uint8_t addr, uint32_t timeout) 
{
  while(I2C1->STAR2 & I2C_STAR2_BUSY);            // wait until bus ready
  I2C1->CTLR1 |= I2C_CTLR1_START                  // set START condition
               | I2C_CTLR1_ACK;                   // set ACK
  while(!(I2C1->STAR1 & I2C_STAR1_SB));           // wait for START generated
  I2C1->DATAR = addr;                             // send slave address + R/W bit
  uint32_t t = timeout;
  while(!(I2C1->STAR1 & I2C_STAR1_ADDR) || (t-- > 0));         // wait for address transmitted with timeout
  uint16_t reg = I2C1->STAR2;                     // clear flags
  I2C_rwflag = addr & 1;                          // set read/write flag
}
#pragma GCC diagnostic pop

// Send data byte via I2C bus
void I2C_write(uint8_t data) {
  while(!(I2C1->STAR1 & I2C_STAR1_TXE));          // wait for last byte transmitted
  I2C1->DATAR = data;                             // send data byte
}

// Read data byte via I2C bus (ack=0 for last byte, ack>0 if more bytes to follow)
uint8_t I2C_read(uint8_t ack) {
  if(!ack) {                                      // last byte?
    I2C1->CTLR1 &= ~I2C_CTLR1_ACK;                // -> set NAK
    I2C1->CTLR1 |=  I2C_CTLR1_STOP;               // -> set STOP condition
  }
  while(!(I2C1->STAR1 & I2C_STAR1_RXNE));         // wait for data byte received
  return I2C1->DATAR;                             // return received data byte
}

// Stop I2C transmission
void I2C_stop(void) {
    if(!I2C_rwflag) {                               // only if not already stopped
    while(!(I2C1->STAR1 & I2C_STAR1_BTF));        // wait for last byte transmitted
    I2C1->CTLR1 |= I2C_CTLR1_STOP;                // set STOP condition
  }
}

// Send data buffer via I2C bus using DMA
void I2C_writeBuffer(uint8_t* buf, uint16_t len) {
  DMA1_Channel6->CNTR  = len;                     // number of bytes to be transfered
  DMA1_Channel6->MADDR = (uint32_t)buf;           // memory address
  DMA1_Channel6->CFGR |= DMA_CFGR6_EN;             // enable DMA channel
  I2C1->CTLR2         |= I2C_CTLR2_DMAEN;         // enable DMA request
}

// Interrupt service routine
void DMA1_Channel6_IRQHandler(void) INTERRUPT_DECORATOR;
void DMA1_Channel6_IRQHandler(void) {
  I2C1->CTLR2         &= ~I2C_CTLR2_DMAEN;        // disable DMA request
  DMA1_Channel6->CFGR &= ~DMA_CFGR6_EN;            // disable DMA channel
  DMA1->INTFCR         = DMA_CGIF6;               // clear interrupt flags
  while(!(I2C1->STAR1 & I2C_STAR1_BTF));          // wait for last byte transmitted
  I2C1->CTLR1         |= I2C_CTLR1_STOP;          // set STOP condition
}

// Read data via I2C bus to buffer and stop
void I2C_readBuffer(uint8_t* buf, uint16_t len) {
  while(len--) *buf++ = I2C_read(len > 0);
}

uint8_t  I2C_readReg8(uint8_t addr, uint8_t reg) 
{
	uint8_t tmp;
	I2C_startWrite(addr);
	I2C_write(reg);
  I2C_stop();
  I2C_startRead(addr);
	tmp = I2C_read(0); 
	return tmp;
}

void I2C_writeReg8(uint8_t addr, uint8_t reg, uint8_t value) 
{
	I2C_startWrite(addr); 
	I2C_write(reg);
	I2C_write(value);
	I2C_stop();
}

void I2C_readMulti(uint8_t addr, uint8_t reg, uint8_t * dst, uint8_t count)
{
  for(uint8_t i = 0; i < count; i++)
  {
    dst[i] = I2C_readReg8(addr, reg++);
  }
}

static inline i2c_err_t i2c_error(void)
{
	if(I2C1->STAR1 & I2C_STAR1_BERR)  {I2C1->STAR1 &= ~I2C_STAR1_BERR;  return I2C_ERR_BERR;}
	if(I2C1->STAR1 & I2C_STAR1_AF)    {I2C1->STAR1 &= ~I2C_STAR1_AF;    return I2C_ERR_NACK;}
	if(I2C1->STAR1 & I2C_STAR1_ARLO)  {I2C1->STAR1 &= ~I2C_STAR1_ARLO;  return I2C_ERR_ARLO;}
	if(I2C1->STAR1 & I2C_STAR1_OVR)   {I2C1->STAR1 &= ~I2C_STAR1_OVR;   return I2C_ERR_OVR;}

	return I2C_OK;
}

bool i2c_ping(const uint8_t addr)
{
	// Send the address and get the status
	I2C_startWrite(addr);

  i2c_err_t i2c_ret = i2c_error();

	// Signal a STOP without wait
	I2C1->CTLR1 |= I2C_CTLR1_STOP;

	return i2c_ret == I2C_OK;
}