#ifndef PWMMOSFETS_H
#define PWMMOSFETS_H

#include <Arduino.h>
#include "../../../SharedMemory/sharedMemory.h"
#include "../GPIO_Definition.h"
#include "../../../Tasks/Debug/debug.h"

#define MOSFETCTRL_PIN_RECIRCULATEPUMP COOLEDMATTRESSIO_PIN2
#define MOSFETCTRL_PIN_MATTRESSPUMP    COOLEDMATTRESSIO_PIN33
#define MOSFETCTRL_PIN_PELTIERFANS     COOLEDMATTRESSIO_PIN0


class PowerMosfets {
private:
    uint8_t powermosfet_pin;
    uint8_t powermosfet_dutyCyle;

public:
    // Construtor
    PowerMosfets(uint8_t pin) : powermosfet_pin(pin),powermosfet_dutyCyle(0) {}

    // Métodos de acesso (getters)
    uint8_t getPin() const {
        return powermosfet_pin;
    }
    uint8_t getDutyCycle() {
        return powermosfet_dutyCyle;
    }
    
    // Métodos de modificação (setters)
    void init(){
        pinMode(powermosfet_pin, OUTPUT);
    }
    void setDutyCycle(uint8_t dutyCycle) {
        powermosfet_dutyCyle = dutyCycle;
        analogWrite(powermosfet_pin,powermosfet_dutyCyle);
    }
};

void initPowerMosfets();
void powerMosfetsTask(void* functionInputParam);


#endif