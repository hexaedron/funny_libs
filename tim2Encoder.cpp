#include "tim2Encoder.h"

tim2Encoder::tim2Encoder(const uint32_t remapMode = AFIO_PCFR1_TIM2_REMAP_NOREMAP)
{
    this->init(remapMode);
    this->count      = TIM2->CNT >> 2;
    this->last_count = TIM2->CNT >> 2;
}

void tim2Encoder::init(const uint32_t remapMode = AFIO_PCFR1_TIM2_REMAP_NOREMAP)
{
    // Enable TIM2 and AFIO *very important!*
	RCC->APB2PCENR |= RCC_APB2Periph_AFIO;
	RCC->APB1PCENR |= RCC_APB1Periph_TIM2;

	AFIO->PCFR1 |= remapMode; //set remap mode 

	switch (remapMode) // We need to configure pins according to remap method 
	{
		case AFIO_PCFR1_TIM2_REMAP_NOREMAP:

			RCC->APB2PCENR |= RCC_APB2Periph_GPIOD;
			
			// PD4 is T2CH1_, Input w/ Pullup/down
			GPIOD->CFGLR &= ~(0xf<<(4*4)); //clear old values
			GPIOD->CFGLR |= (GPIO_CNF_IN_PUPD)<<(4*4); //set new ones
			//1 = pull-up, 0 = pull-down
			GPIOD->OUTDR |= 1<<4;

			// PD3 is T2CH2_, Input w/ Pullup/down
			GPIOD->CFGLR &= ~(0xf<<(4*3)); //clear values
			GPIOD->CFGLR |= (GPIO_CNF_IN_PUPD)<<(4*3); //set new ones
			//1 = pull-up, 0 = pull-down
			GPIOD->OUTDR |= 1<<3;

		break;

		case AFIO_PCFR1_TIM2_REMAP_PARTIALREMAP1:

			RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;

			// PC5 is T2CH1_, Input w/ Pullup/down
			GPIOC->CFGLR &= ~(0xf<<(4*5)); //clear old values
			GPIOC->CFGLR |= (GPIO_CNF_IN_PUPD)<<(4*5); //set new ones
			//1 = pull-up, 0 = pull-down
			GPIOC->OUTDR |= 1<<5;

			// PC2 is T2CH2_, Input w/ Pullup/down
			GPIOC->CFGLR &= ~(0xf<<(4*2)); //clear values
			GPIOC->CFGLR |= (GPIO_CNF_IN_PUPD)<<(4*2); //set new ones
			//1 = pull-up, 0 = pull-down
			GPIOC->OUTDR |= 1<<2;
		break;

		case AFIO_PCFR1_TIM2_REMAP_PARTIALREMAP2:

			RCC->APB2PCENR |= RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD;

			// PC1 is T2CH1_, Input w/ Pullup/down
			GPIOC->CFGLR &= ~(0xf<<(4*1)); //clear old values
			GPIOC->CFGLR |= (GPIO_CNF_IN_PUPD)<<(4*1); //set new ones
			//1 = pull-up, 0 = pull-down
			GPIOC->OUTDR |= 1<<1;

			// PD3 is T2CH2_, Input w/ Pullup/down
			GPIOD->CFGLR &= ~(0xf<<(4*3)); //clear values
			GPIOD->CFGLR |= (GPIO_CNF_IN_PUPD)<<(4*3); //set new ones
			//1 = pull-up, 0 = pull-down
			GPIOD->OUTDR |= 1<<3;

		break;

		case AFIO_PCFR1_TIM2_REMAP_FULLREMAP:
			
			RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;
			
			// PC1 is T2CH1_, Input w/ Pullup/down
			GPIOC->CFGLR &= ~(0xf<<(4*1)); //clear old values
			GPIOC->CFGLR |= (GPIO_CNF_IN_PUPD)<<(4*1); //set new ones
			//1 = pull-up, 0 = pull-down
			GPIOC->OUTDR |= 1<<1;

			// PC7 is T2CH2_, Input w/ Pullup/down
			GPIOC->CFGLR &= ~(0xf<<(4*7)); //clear values
			GPIOC->CFGLR |= (GPIO_CNF_IN_PUPD)<<(4*7); //set new ones
			//1 = pull-up, 0 = pull-down
			GPIOC->OUTDR |= 1<<7;

		break;
		
		default:
		break;
	}
	
	
	// Reset TIM2 to init all regs
	RCC->APB1PRSTR |= RCC_APB1Periph_TIM2;
	RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM2;
	
	// set TIM2 clock prescaler If you want to reduce the resolution of the encoder
	//TIM2->PSC = 0x0000;

	// set a automatic reload if you want the counter to wrap earlier than 0xffff
	//TIM2->ATRLR = 0xffff;

	// SMCFGR: set encoder mode SMS=011b
	TIM2->SMCFGR |= TIM_EncoderMode_TI12;

	// initialize timer
	TIM2->SWEVGR |= TIM_UG;

	// set count to about mid-scale to avoid wrap-around
	TIM2->CNT = 0x8fff;

	// Enable TIM2
	TIM2->CTLR1 |= TIM_CEN;
}

int32_t tim2Encoder::getDelta(void)
{
    this->count = TIM2->CNT >> 2;
    int32_t tmp = (int32_t)this->count - this->last_count;
    this->last_count = this->count;
    return tmp;
}