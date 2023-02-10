#include "stm32l476xx.h"
#include "libs/SysTickTimer.h"
#include "libs/defines.h"
#include "libs/IO_Digital.h"

#define SEMIHOSTING

#ifdef SEMIHOSTING
#include <stdio.h>
#endif

/*
 * TODO: Сделать обёртку вокруг RCC init
 */

void RCC_DeInit(void);

void RCC_Init(void);


#ifdef SEMIHOSTING
extern "C" void initialise_monitor_handles(void);
#endif

int main(void) {
    RCC_DeInit();
    RCC_Init();
    SysTickTimer::init(SystemCoreClock);
#ifdef SEMIHOSTING
    initialise_monitor_handles();
#endif

    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);
    while (!READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN));
    IO_Digital pb2(GPIOB, 2, IO_Digital::OUT, IO_Digital::NOPULL);


    while (1) {
        SysTickTimer::delay_ms(500);
        pb2.toggleState();
        SysTickTimer::delay_ms(500);
        pb2.toggleState();
#ifdef SEMIHOSTING
        printf("Hello, World!\n");
#endif
    }

    return 0;
}

void RCC_DeInit(void) {
    //enable HSI
    SET_BIT(RCC->CR, RCC_CR_HSION);
    while (!READ_BIT(RCC->CR, RCC_CR_HSIRDY)) {}
    //reset calibration
    CLEAR_BIT(RCC->ICSCR, 0xFF << RCC_ICSCR_HSITRIM_Pos);
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
    SET_BIT(RCC->CFGR, RCC_CFGR_SW_0);

}

//set 80MHz HCLK and SYSCLK
void RCC_Init(void) {
    //Flash config
    //Instruction cache enable
    SET_BIT(FLASH->ACR, FLASH_ACR_ICEN);
    //Prefetch enable
    SET_BIT(FLASH->ACR, FLASH_ACR_PRFTEN);
    //Latency for 80MHz 4WS
    SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY_4WS);

    //RCC config
    //APB1 and APB2 prescalers to NOT DIVIDED
    SET_BIT(RCC->CFGR, RCC_CFGR_PPRE1_DIV1);
    SET_BIT(RCC->CFGR, RCC_CFGR_PPRE2_DIV1);
    //Set voltage range for High performance(RANGE 1)
    PWR->CR1 = (PWR->CR1 & ~PWR_CR1_VOS_1) | PWR_CR1_VOS_0;
    while (PWR->SR2 & PWR_SR2_VOSF);
    //Set AHB prescaler to 0
    RCC->CFGR &= ~(RCC_CFGR_HPRE_Msk);
    //PLL config CLEAR
    CLEAR_REG(RCC->PLLCFGR);
    //PLL set multiplication
    RCC->PLLCFGR |= 0b1010 << RCC_PLLCFGR_PLLN_Pos;
    //PLL source set
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_HSI);
    //Enable PLLCLK main
    SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_PLLREN);
    //Enable PLL and wait unlock
    SET_BIT(RCC->CR, RCC_CR_PLLON);
    while (!(RCC->CR & RCC_CR_PLLRDY));
    //Change source to PLLCLK
    SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);
    while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);
    CLEAR_BIT(RCC->CR, RCC_CR_MSION);
    SystemCoreClockUpdate();
}