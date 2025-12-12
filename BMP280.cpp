#include "i2c_dma.h"
#include "BMP280.h"

void bmp280::init_default_params() 
{
	this->bmp280_handle.params.mode = BMP280_MODE_NORMAL;
	this->bmp280_handle.params.filter = BMP280_FILTER_OFF;
	this->bmp280_handle.params.oversampling_pressure = BMP280_STANDARD;
	this->bmp280_handle.params.oversampling_temperature = BMP280_STANDARD;
	this->bmp280_handle.params.oversampling_humidity = BMP280_STANDARD;
	this->bmp280_handle.params.standby = BMP280_STANDBY_250;
}


void bmp280::sleep() 
{
	this->bmp280_handle.params.mode = BMP280_MODE_SLEEP;
}


void bmp280::read_calibration_data() 
{

	this->bmp280_handle.dig_T1 = I2C_readReg8(this->i2cAddress, 0x89)<<8 | I2C_readReg8(this->i2cAddress, 0x88);
	this->bmp280_handle.dig_T2 = I2C_readReg8(this->i2cAddress, 0x8b)<<8 | I2C_readReg8(this->i2cAddress, 0x8a);
	this->bmp280_handle.dig_T3 = I2C_readReg8(this->i2cAddress, 0x8d)<<8 | I2C_readReg8(this->i2cAddress, 0x8c);
	this->bmp280_handle.dig_P1 = I2C_readReg8(this->i2cAddress, 0x8f)<<8 | I2C_readReg8(this->i2cAddress, 0x8e);
	this->bmp280_handle.dig_P2	= I2C_readReg8(this->i2cAddress, 0x91)<<8 | I2C_readReg8(this->i2cAddress, 0x90);
	this->bmp280_handle.dig_P3	= I2C_readReg8(this->i2cAddress, 0x93)<<8 | I2C_readReg8(this->i2cAddress, 0x92);
	this->bmp280_handle.dig_P4	= I2C_readReg8(this->i2cAddress, 0x95)<<8 | I2C_readReg8(this->i2cAddress, 0x94);
	this->bmp280_handle.dig_P5	= I2C_readReg8(this->i2cAddress, 0x97)<<8 | I2C_readReg8(this->i2cAddress, 0x96);
	this->bmp280_handle.dig_P6	= I2C_readReg8(this->i2cAddress, 0x99)<<8 | I2C_readReg8(this->i2cAddress, 0x98);
	this->bmp280_handle.dig_P7	= I2C_readReg8(this->i2cAddress, 0x9b)<<8 | I2C_readReg8(this->i2cAddress, 0x9a);
	this->bmp280_handle.dig_P8	= I2C_readReg8(this->i2cAddress, 0x9d)<<8 | I2C_readReg8(this->i2cAddress, 0x9c);
	this->bmp280_handle.dig_P9	= I2C_readReg8(this->i2cAddress, 0x9f)<<8 | I2C_readReg8(this->i2cAddress, 0x9e);

}

void bmp280::read_hum_calibration_data() 
{
	uint8_t e4,e5,e6;

	this->bmp280_handle.dig_H1 = I2C_readReg8(this->i2cAddress, 0xa1);
	this->bmp280_handle.dig_H2	=	I2C_readReg8(this->i2cAddress, 0xe2)<<8 | I2C_readReg8(this->i2cAddress, 0xe1);
	this->bmp280_handle.dig_H3	=	I2C_readReg8(this->i2cAddress, 0xe3);
	
					e4	=	I2C_readReg8(this->i2cAddress, 0xe4);
					e5	=	I2C_readReg8(this->i2cAddress, 0xe5);
					e6  =	I2C_readReg8(this->i2cAddress, 0xe6);
	this->bmp280_handle.dig_H4 = (e4<<4) | (e5 & 0xf);
	this->bmp280_handle.dig_H5 = (e6<<4)	| (e5>>4);
	this->bmp280_handle.dig_H6	=	I2C_readReg8(this->i2cAddress, 0xe7);
	
}


bool bmp280::init() 
{

	this->bmp280_handle.id = I2C_readReg8(this->i2cAddress, BMP280_REG_ID);
	if (this->bmp280_handle.id != BMP280_CHIP_ID && this->bmp280_handle.id != BME280_CHIP_ID) 
	{
	return false;
	}
	I2C_writeReg8(this->i2cAddress, BMP280_REG_RESET, BMP280_RESET_VALUE);	// Soft reset.
	while(I2C_readReg8(this->i2cAddress, BMP280_REG_STATUS)&0x01){};				// Wait until finished copying over the NVP data.
	
	this->read_calibration_data();
	this->read_hum_calibration_data();

	uint8_t config = (this->bmp280_handle.params.standby << 5) | (this->bmp280_handle.params.filter << 2);
	I2C_writeReg8(this->i2cAddress, BMP280_REG_CONFIG, config);

	if (this->bmp280_handle.params.mode == BMP280_MODE_FORCED) {
		this->bmp280_handle.params.mode = BMP280_MODE_SLEEP;  // initial mode for forced is sleep
	}

	uint8_t ctrl = (this->bmp280_handle.params.oversampling_temperature << 5)
			| (this->bmp280_handle.params.oversampling_pressure << 2) | (this->bmp280_handle.params.mode);

	if (this->bmp280_handle.id == BME280_CHIP_ID) 
	{
		// Write crtl hum reg first, only active after write to BMP280_REG_CTRL.
		uint8_t ctrl_hum = this->bmp280_handle.params.oversampling_humidity;
		I2C_writeReg8(this->i2cAddress, BMP280_REG_CTRL_HUM, ctrl_hum);
	}
	
	I2C_writeReg8(this->i2cAddress, BMP280_REG_CTRL, ctrl);
	return true;
}

void bmp280::force_measurement() 
{
	uint8_t ctrl;
	ctrl = I2C_readReg8(this->i2cAddress, BMP280_REG_CTRL);
	ctrl &= ~3;  // clear two lower bits
	ctrl |= BMP280_MODE_FORCED;
	I2C_writeReg8(this->i2cAddress, BMP280_REG_CTRL, ctrl);
}

bool bmp280::is_measuring() 
{
	uint8_t status;
	status = I2C_readReg8(this->i2cAddress, BMP280_REG_STATUS);
	if (status & (1 << 3)) {
		return true;
	}
	return false;
}

/**
 * Compensation algorithm is taken from BMP280 datasheet.
 *
 * Return value is in degrees Celsius.
 */
int32_t bmp280::compensate_temperature(int32_t adc_T,	int32_t *t_fine) 
{
	int32_t var1, var2, T;

  var1  = ((((adc_T>>3) - ((int32_t)this->bmp280_handle.dig_T1<<1))) * ((int32_t)this->bmp280_handle.dig_T2)) >> 11; 
  var2  = (((((adc_T>>4) - (int32_t)this->bmp280_handle.dig_T1)) * ((adc_T>>4) - ((int32_t)this->bmp280_handle.dig_T1)))>> 12) *((int32_t)this->bmp280_handle.dig_T3) >> 14; 
  *t_fine = var1 + var2; 
  T  = (*t_fine * 5 + 128) >> 8; 
  return T; 
}

/**
 * Compensation algorithm is taken from BMP280 datasheet.
 *
 * Return value is in Pa, 24 integer bits and 8 fractional bits.
 */
uint32_t bmp280::compensate_pressure(int32_t adc_press, int32_t t_fine) {
	int64_t var1, var2, p;

	var1 = ((int64_t) t_fine) - 128000;
	var2 = var1 * var1 * (int64_t) this->bmp280_handle.dig_P6;
	var2 = var2 + ((var1 * (int64_t) this->bmp280_handle.dig_P5) << 17);
	var2 = var2 + (((int64_t) this->bmp280_handle.dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t) this->bmp280_handle.dig_P3) >> 8)
			+ ((var1 * (int64_t) this->bmp280_handle.dig_P2) << 12);
	var1 = (((int64_t) 1 << 47) + var1) * ((int64_t) this->bmp280_handle.dig_P1) >> 33;

	if (var1 == 0) {
		return 0;  // avoid exception caused by division by zero
	}

	p = 1048576 - adc_press;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = ((int64_t) this->bmp280_handle.dig_P9 * (p >> 13) * (p >> 13)) >> 25;
	var2 = ((int64_t) this->bmp280_handle.dig_P8 * p) >> 19;

	p = ((p + var1 + var2) >> 8) + ((int64_t) this->bmp280_handle.dig_P7 << 4);
	return (uint32_t)p;
}

/**
 * Compensation algorithm is taken from BME280 datasheet.
 *
 * Return value is in Pa, 24 integer bits and 8 fractional bits.
 */
uint32_t __attribute__((optimize("O3"))) bmp280::compensate_humidity(int32_t adc_hum, int32_t t_fine) {
	int32_t v_x1_u32r;

	v_x1_u32r = t_fine - (int32_t) 76800;
	v_x1_u32r = ((((adc_hum << 14) - ((int32_t)this->bmp280_handle.dig_H4 << 20)
			- ((int32_t) this->bmp280_handle.dig_H5 * v_x1_u32r)) + (int32_t) 16384) >> 15)
			* (((((((v_x1_u32r * (int32_t)this->bmp280_handle.dig_H6) >> 10)
					* (((v_x1_u32r * (int32_t) this->bmp280_handle.dig_H3) >> 11)
							+ (int32_t) 32768)) >> 10) + (int32_t) 2097152)
					* (int32_t)this->bmp280_handle.dig_H2 + 8192) >> 14);
	v_x1_u32r = v_x1_u32r
			- (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7)
					* (int32_t)this->bmp280_handle.dig_H1) >> 4);
	v_x1_u32r = v_x1_u32r < 0 ? 0 : v_x1_u32r;
	v_x1_u32r = v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r;
	return v_x1_u32r >> 12;
}

// Pa to mmHg conversion (Pascals to millimeters of mercury)
// input:
//   p_pa - pressure, milli pascals
// return: pressure, "micro-meters" of mercury (value of '739503' represents '739.503mmHg')
uint32_t bmp280::Pa_to_mmHg(uint32_t p_pa) 
{
	// 32-bit fixed point calculations

	// Convert milli-Pascals to Pascals (Q32.0),
	// multiply by the magic constant (Q0.22),
	// the product is a pressure in mmHg (Q10.22)
	// note: 1Pa = ~0.00750061683mmHg
	uint32_t p = p_pa / 1000U;
	p *= 31460U; // 31460 is the ~0.00750061683 in Q0.22 format

	// (p_mmHg >> 22) -> integer part from Q10.22 value
	// (p_mmHg << 10) >> 18 -> fractional part truncated down to 14 bits
	// (XXX * 61039) / 1000000 is rough integer equivalent of float (XXX / 16383.0F) * 1000
	return ((p >> 22) * 1000U) + ((((p << 10) >> 18) * 61039U) / 1000000U);
}


void bmp280::read_fixed() {
	int32_t adc_pressure;
	int32_t adc_temp;
	uint8_t data[8];
	int32_t fine_temp;
	
	// Only the BME280 supports reading the humidity.
	if (this->bmp280_handle.id != BME280_CHIP_ID) {this->humidity = 0;}
		
	
	// Need to read in one sequence to ensure they match.
	uint8_t size = this->humidity ? 8 : 6;
	I2C_readMulti(this->i2cAddress, BMP280_REG_PRESS_MSB, data, size);
	
	adc_pressure = data[0] << 12 | data[1] << 4 | data[2] >> 4;
	adc_temp = data[3] << 12 | data[4] << 4 | data[5] >> 4;

	this->temperature = compensate_temperature(adc_temp, &fine_temp);
	
	this->pressure = compensate_pressure(adc_pressure, fine_temp);

	if (this->humidity) {
		int32_t adc_humidity = data[6] << 8 | data[7];
		this->humidity = compensate_humidity(adc_humidity, fine_temp);
	}

}

int32_t bmp280::getTemp()
{
	return this->temperature;
}

uint32_t bmp280::getPressurePa()
{
	return this->pressure;
}

uint32_t bmp280::getPressureMbar()
{
	return (this->pressure + 12800) / 25600;
}

uint32_t bmp280::getPressureMmHg()
{
	return this->Pa_to_mmHg(this->pressure);
}

uint32_t bmp280::getHumidity()
{
	return this->humidity;
}

uint32_t bmp280::getHumidityTenths()
{
	return (this->humidity * 10 + 512) >> 10;
}

void bmp280::setI2CAddress(uint8_t addr)
{
	this->i2cAddress = addr;
}