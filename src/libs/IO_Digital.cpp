//
// Created by root on 10.02.23.
//

#include "IO_Digital.h"

IO_Digital::IO_Digital(GPIO_TypeDef *port, uint32_t pin, IO io, PULL pull) {
    this->port = port;
    this->pin = pin;
    this->io = io;
    this->pull = pull;
    state = STATE::RESET;
    init_pin();
}

IO_Digital::IO_Digital(GPIO_TypeDef *port, uint32_t pin, IO io, PULL pull, STATE state) {
    this->port = port;
    this->pin = pin;
    this->io = io;
    this->pull = pull;
    this->state = state;
    init_pin();
}

constexpr IO_Digital::PULL IO_Digital::getPull() const {
    return pull;
}

constexpr IO_Digital::IO IO_Digital::isIo() const {
    return io;
}

constexpr IO_Digital::STATE IO_Digital::isState() const {
    return state;
}

void IO_Digital::setPull(PULL pull) {
    IO_Digital::pull = pull;
    port->PUPDR &= ~(0x03 << (pin * 2));
    port->PUPDR |= pull << (pin * 2);
}

void IO_Digital::setIo(IO io) {
    IO_Digital::io = io;
    port->MODER &= ~(0x03 << (pin * 2));
    if(io){
        port->MODER |= 0x01 << (pin * 2);
    }
}

void IO_Digital::setState(STATE state) {
    IO_Digital::state = state;
    if(io && state){
        port->BSRR |= 0x01 << pin;
    } else{
        port->BSRR |= 0x01 << (pin + 16);
    }
}

void IO_Digital::toggleState() {
    IO_Digital::state = static_cast<STATE>(~state);
    if(io && state){
        port->BSRR |= 0x01 << pin;
    } else{
        port->BSRR |= 0x01 << (pin + 16);
    }
}

void IO_Digital::init_pin(){
    port->MODER &= ~(0x03 << (pin * 2));
    if(io){
        port->MODER |= 0x01 << (pin * 2);
    }
    port->OTYPER &= ~(0x01 << pin);
    port->OSPEEDR |= 0x03 << (pin * 2);
    port->PUPDR &= ~(0x03 << (pin * 2));
    port->PUPDR |= pull << (pin * 2);
    port->BSRR |= 0x01 << (pin + 16);
    if(io && state){
        port->BSRR |= 0x01 << pin;
    }
}




