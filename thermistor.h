#ifndef _THERMISTOR_H
#define _THERMISTOR_H

#include "ntc_tables.h"
#include "adc.h"

// Resistor values. Each value corresponds to 
// the specific conversion table in ntc_tables.h
typedef enum 
{
    resistor_10K,
    resistor_15K,
    resistor_2_5K,
} resistorValues_t;

class thermistor
{
private:
    uint32_t pin;
    resistorValues_t val;
public:
    thermistor(uint32_t, resistorValues_t);
    int16_t getTemp(void);
};

thermistor::thermistor(uint32_t pin, resistorValues_t val)
{
    this->pin = pin;
    this->val = val;
}

int16_t thermistor::getTemp(void)
{
    uint16_t adcResult;
    adcResult = adc_get(this->pin);

    if(adcResult >= 1022)
		return INT16_MAX;

    switch (this->val)
    {
        case resistor_10K:
            return NTC_10K[adcResult - 1];
        break;

        case resistor_15K:
            return NTC_15K[adcResult - 1];
        break;

        case resistor_2_5K:
            return NTC_2_5K[adcResult - 1];
        break;
    }

    return INT16_MAX;
}


#endif