//
// Created by root on 10.02.23.
//

#include "SysTickTimer.h"

uint32_t SysTickTimer::ticks = 0;

void SysTickTimer::init(uint32_t freq) {
    // я не понимаю почему мы получает 1кГц таким образом, ведь 80 000 000 / 1000 = 80 000 тиков перед прерыванием
    SysTick_Config(freq / 1000);
    ticks = 0;
}

uint32_t SysTickTimer::get_ticks() {
    return ticks;
}

void SysTickTimer::delay_ms(uint32_t ms) {
    uint32_t start, end;
    start = get_ticks();
    end = start + ms;
    if (start < end) {
        while ((get_ticks() >= start) && (get_ticks() < end)) {
            // do nothing
        }
    } else {
        while ((get_ticks() >= start) || (get_ticks() < end)) {
            // do nothing
        };
    }
}

extern "C" {
void SysTick_Handler(void) {
    SysTickTimer::ticks++;
}
}
