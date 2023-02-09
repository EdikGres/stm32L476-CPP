#include "stm32l476xx.h"

#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))


void RCC_DeInit(void)
{	
	//enable HSI
	SET_BIT(RCC->CR, RCC_CR_HSION);
	while(!READ_BIT(RCC->CR, RCC_CR_HSIRDY)) {}
	
	//reset calibration
	CLEAR_BIT(RCC->ICSCR, 0xFF00);
	
	CLEAR_REG(RCC->CFGR);
	
	while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != 0) {}
	 
	CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
	
	while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != 0) {}
	
	CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
	
	while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != 0) {}
	
	CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
	
	
	//INTERRUPTs clear
	SET_BIT(RCC->CICR, 0x7FF);
	
	//disable interrupts
	CLEAR_REG(RCC->CIER);
	
	
}



int main (void){

	RCC_DeInit();

	SystemCoreClockUpdate();

	SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);
	while (!READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN))
		;

	GPIOB->MODER &= ~(GPIO_MODER_MODE2_0);
	GPIOB->MODER &= ~(GPIO_MODER_MODE2_1);
	
	GPIOB->MODER |= GPIO_MODER_MODE2_0;
	GPIOB->OTYPER &= 0;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED2_1 & GPIO_OSPEEDR_OSPEED2_0;
	GPIOB->BSRR |= GPIO_BSRR_BS2; 
	
	while(1){
		
		for(volatile uint32_t i = 0; i < 250000; i++){
			asm("NOP");
		}
		GPIOB->ODR &= ~GPIO_ODR_OD2;
		for(volatile uint32_t i = 0; i < 250000; i++){
			asm("NOP");
		}
		GPIOB->ODR |= GPIO_ODR_OD2;
	
	}

	return 0;
}
