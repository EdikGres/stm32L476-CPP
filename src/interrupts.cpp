//
// Created by root on 10.02.23.
//
#include "stm32l476xx.h"

extern "C" {
void HardFault_Handler(void) {
    while (1);
}
void MemManage_Handler(void) {
    while (1);
}
void BusFault_Handler(void) {
    while (1);
}
//volatile uint32_t ticks = 0;




}