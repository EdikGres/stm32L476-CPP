//
// Created by root on 10.02.23.
//

#ifndef STM32_TEMPLATE_SYSTICKTIMER_H
#define STM32_TEMPLATE_SYSTICKTIMER_H

#include "stm32l476xx.h"

class SysTickTimer {
public:
    static uint32_t ticks;

    static void init(uint32_t freq);

    static uint32_t get_ticks();

    static void delay_ms(uint32_t ms);

};


#endif //STM32_TEMPLATE_SYSTICKTIMER_H
