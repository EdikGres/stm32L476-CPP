//
// Created by root on 10.02.23.
//

#ifndef STM32_TEMPLATE_IO_DIGITAL_H
#define STM32_TEMPLATE_IO_DIGITAL_H

#include "stm32l476xx.h"

//TODO: Доработать прерывания для пина!
//TODO: Сделать включение тактирования шины
//TODO: Сделать настройку скорости работы пино
//TODO: Сделать тесты

/**
 * @brief GPIO sets class
 */
class IO_Digital {
public:
    enum PULL {
        NOPULL, //No pull-up, pull-down
        PULLUP, //Pull-up
        PULLDOWN //Pull-down
    };
    enum IO {
        IN,
        OUT,
    };
    enum STATE {
        RESET,
        SET
    };

    /**
     * Create a new GPIO object
     * @param port PORTx struct of \ref GPIO_TypeDef
     * @param pin Number pin
     * @param io Input/Out states. true - out, false - in.
     * @param pull Pull, \ref IO_Digital::PULL::NOPULL
     */
    IO_Digital(GPIO_TypeDef *port, uint32_t pin, IO io, PULL pull);

    /**
     * Create a new GPIO object with init state
     * @param port PORTx struct of \ref GPIO_TypeDef
     * @param pin Number pin
     * @param io Input/Out states. true - out, false - in.
     * @param pull Pull, \ref IO_Digital::PULL::NOPULL
     * @param state Init state, true - SET, false - RESET
     */
    IO_Digital(GPIO_TypeDef *port, uint32_t pin, IO io, PULL pull, STATE state);

    virtual ~IO_Digital();

    constexpr PULL getPull() const;

    constexpr IO isIo() const;

    constexpr STATE isState() const;

    void setPull(PULL pull);

    void setIo(IO io);

    void setState(STATE state);

    void toggleState();


private:
    GPIO_TypeDef *port; //PORTx
    uint32_t pin; //GPIO_Pin
    PULL pull; //Pull 0 - no pull-up/down, 1 - pull-up, 2- pull-down
    IO io; //true - out, false - in
    STATE state; // on off

    void init_pin();
};


#endif //STM32_TEMPLATE_IO_DIGITAL_H
