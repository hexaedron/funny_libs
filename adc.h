#ifndef _ADC_H
#define _ADC_H

#include "ch32fun.h"

constexpr uint32_t pin2ADC(uint32_t pin)
{
	switch (pin)
	{
	case PA2:
		return 0;
	break;

	case PA1:
		return 1;
	break;

	case PC4:
		return 2;
	break;

	case PD2:
		return 3;
	break;

	case PD3:
		return 4;
	break;

	case PD5:
		return 5;
	break;

	case PD6:
		return 6;
	break;

	case PD7:
		return 7;
	break;
	
	default:
		return 0;
	break;
	}
}

void adc_init(uint32_t channelConfig)
{
	// ADCCLK = 24 MHz => RCC_ADCPRE = 0: divide by 2
	RCC->CFGR0 &= ~(0x1F<<11);
	
	// Enable ADC
	RCC->APB2PCENR |= RCC_APB2Periph_ADC1;
	
	// Reset the ADC to init all regs
	RCC->APB2PRSTR |= RCC_APB2Periph_ADC1;
	RCC->APB2PRSTR &= ~RCC_APB2Periph_ADC1;
	
	// Reset conversions
	ADC1->RSQR1 = 0;
	ADC1->RSQR2 = 0;
	ADC1->RSQR3 = 0;
	
	// set sampling time for channels
	ADC1->SAMPTR2 |= channelConfig;  
		
	// turn on ADC and set rule group to sw trig
	ADC1->CTLR2 |= ADC_ADON | ADC_EXTSEL;
	
	// Reset calibration
	ADC1->CTLR2 |= ADC_RSTCAL;
	while(ADC1->CTLR2 & ADC_RSTCAL);
	
	// Calibrate
	ADC1->CTLR2 |= ADC_CAL;
	while(ADC1->CTLR2 & ADC_CAL);
	
	// should be ready for SW conversion now
}


uint16_t adc_get(uint32_t pin)
{
	// Here we select the desired channel
    ADC1->RSQR3 = pin2ADC(pin);
	
	// start sw conversion (auto clears)
	ADC1->CTLR2 |= ADC_SWSTART;
	
	// wait for conversion complete
	while(!(ADC1->STATR & ADC_EOC));
	
	// get result
	return ADC1->RDATAR;
}

#endif