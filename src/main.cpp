#include "stm32l476xx.h"

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))


int main (void){
	SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);
	while (!READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN))
		;

	GPIOB->MODER &= ~(GPIO_MODER_MODE2_0);
	GPIOB->MODER &= ~(GPIO_MODER_MODE2_1);
	
	GPIOB->MODER |= GPIO_MODER_MODE2_0;
	GPIOB->OTYPER &= 0;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED2_1 & GPIO_OSPEEDR_OSPEED2_0;
	GPIOB->BSRR |= GPIO_BSRR_BS2; 

	return 0;
}
