#ifndef HBRIDGE_H
#define HBRIDGE_H

#include <Arduino.h>
#include "../../../SharedMemory/sharedMemory.h"
#include "../../../Constants/constants.h"
#include "../GPIO_Definition.h"

#define HBRIDGE_PIN_PELTIERCELL1FWD   COOLEDMATTRESSIO_PIN4
#define HBRIDGE_PIN_PELTIERCELL1REV   COOLEDMATTRESSIO_PIN12
#define HBRIDGE_PIN_PELTIERCELL2FWD   COOLEDMATTRESSIO_PIN13
#define HBRIDGE_PIN_PELTIERCELL2REV   COOLEDMATTRESSIO_PIN14
#define HBRIDGE_PIN_PELTIERCELL3FWD   COOLEDMATTRESSIO_PIN15
#define HBRIDGE_PIN_PELTIERCELL3REV   COOLEDMATTRESSIO_PIN26
#define HBRIDGE_PIN_PELTIERCELL4FWD   COOLEDMATTRESSIO_PIN27
#define HBRIDGE_PIN_PELTIERCELL4REV   COOLEDMATTRESSIO_PIN32

#define PELTIER_STATE_DISABLING 4

//task running at 500ms, so if any peltier cell is disabled, 
//it stays on that disabled phase for at least 30 seconds
#define NUMBER_OF_CYCLES_TO_AVOID_REPETITIVE_PELTIER_ONOFF_CYCLES 60

class HBridge {
private:
    uint8_t hbridge_fwdPin;
    uint8_t hbridge_revPin;
    uint8_t hbridge_desiredValue;

public:
    // Construtor
    HBridge(uint8_t fwdpin,uint8_t revpin) : hbridge_fwdPin(fwdpin),hbridge_revPin(revpin),hbridge_desiredValue(PELTIER_OFF) {}

    // Métodos de acesso (getters)
    uint8_t getFwdPin() const {
        return hbridge_fwdPin;
    }
    uint8_t getRevPin() const {
        return hbridge_revPin;
    }
    uint8_t getDesiredValue() {
        return hbridge_desiredValue;
    }
    
    // Métodos de modificação (setters)
    void init(){
        pinMode(hbridge_fwdPin, OUTPUT);
        pinMode(hbridge_revPin, OUTPUT);
        digitalWrite(hbridge_fwdPin,LOW);
        digitalWrite(hbridge_revPin,LOW);
    }
    void setDesiredValue(uint8_t desiredValue) {
        hbridge_desiredValue = desiredValue;
        switch(hbridge_desiredValue){
            case PELTIER_COOLING_ON:{
                digitalWrite(hbridge_fwdPin,1);
                digitalWrite(hbridge_revPin,0);
            }break;
            case PELTIER_HEATING_ON:{
                digitalWrite(hbridge_fwdPin,0);
                digitalWrite(hbridge_revPin,1);
            }break;
            case PELTIER_OFF:
            default:{
                digitalWrite(hbridge_fwdPin,0);
                digitalWrite(hbridge_revPin,0);
            }break;
        }
    }
};

void initHBridge();
void hBridgeTask(void* functionInputParam);


#endif