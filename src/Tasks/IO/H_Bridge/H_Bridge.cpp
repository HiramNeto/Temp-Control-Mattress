#include "H_Bridge.h"

HBridge PeltierCell1(HBRIDGE_PIN_PELTIERCELL1FWD,HBRIDGE_PIN_PELTIERCELL1REV);
HBridge PeltierCell2(HBRIDGE_PIN_PELTIERCELL2FWD,HBRIDGE_PIN_PELTIERCELL2REV);
HBridge PeltierCell3(HBRIDGE_PIN_PELTIERCELL3FWD,HBRIDGE_PIN_PELTIERCELL3REV);
HBridge PeltierCell4(HBRIDGE_PIN_PELTIERCELL4FWD,HBRIDGE_PIN_PELTIERCELL4REV);

void drivePeltierCell(uint8_t* l_actualPeltierState_pu8, uint8_t* l_desiredPeltierState_pu8, HBridge* l_targetHbridge_pHB, uint16_t* l_peltierStateTransitionCounter_pu16);

void initHBridge(){
    PeltierCell1.init();
    PeltierCell2.init();
    PeltierCell3.init();
    PeltierCell4.init();
}

void drivePeltierCell(uint8_t* l_actualPeltierState_pu8, uint8_t* l_desiredPeltierState_pu8, HBridge* l_targetHbridge_pHB, uint16_t* l_peltierStateTransitionCounter_pu16){
    switch(*l_actualPeltierState_pu8){
        default:
        case PELTIER_UNDEFINED:
        case PELTIER_OFF:{
            //Reset the state transitioning counter
            *l_peltierStateTransitionCounter_pu16 = 0;
            //Drive the peltier cell to the actual state value
            l_targetHbridge_pHB->setDesiredValue(PELTIER_OFF);
            //Go to next state if needed
            *l_actualPeltierState_pu8 = *l_desiredPeltierState_pu8;
        }break;
        case PELTIER_HEATING_ON:
        case PELTIER_COOLING_ON:{
            //Drive the peltier cell to the actual state value
            l_targetHbridge_pHB->setDesiredValue(*l_actualPeltierState_pu8);
            //If any state change is desired, we must first go through a disabling phase, to avoid to much repetitive on/off cycles
            if(*l_desiredPeltierState_pu8!=*l_actualPeltierState_pu8){
                *l_actualPeltierState_pu8 = PELTIER_STATE_DISABLING;
            }
        }break;
        case PELTIER_STATE_DISABLING:{
            l_targetHbridge_pHB->setDesiredValue(PELTIER_OFF);
            *l_peltierStateTransitionCounter_pu16++;
            if(*l_peltierStateTransitionCounter_pu16>NUMBER_OF_CYCLES_TO_AVOID_REPETITIVE_PELTIER_ONOFF_CYCLES){
                *l_actualPeltierState_pu8 = PELTIER_OFF;
            }
        }break;   
    }
}

void hBridgeTask(void* functionInputParam){
    ControllerParameters *functionLocalControllerParameters = (ControllerParameters*)functionInputParam;
    while(1){
        static uint8_t ls_peltier1State_u8 = PELTIER_OFF;
        static uint8_t ls_peltier2State_u8 = PELTIER_OFF;
        static uint8_t ls_peltier3State_u8 = PELTIER_OFF;
        static uint8_t ls_peltier4State_u8 = PELTIER_OFF;
        static uint16_t ls_peltier1StateTransitionCounter_u16 = 0;
        static uint16_t ls_peltier2StateTransitionCounter_u16 = 0;
        static uint16_t ls_peltier3StateTransitionCounter_u16 = 0;
        static uint16_t ls_peltier4StateTransitionCounter_u16 = 0;
        uint8_t l_desiredPeltier1State_u8;
        uint8_t l_desiredPeltier2State_u8;
        uint8_t l_desiredPeltier3State_u8;
        uint8_t l_desiredPeltier4State_u8;
        //Peltier 1
        if (!readProtectedVariable(l_desiredPeltier1State_u8, &functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex)) {
            ls_peltier1State_u8 = PELTIER_UNDEFINED;
        }
        //Peltier 2
        if (!readProtectedVariable(l_desiredPeltier2State_u8, &functionLocalControllerParameters->actuators.heatExchangerPeltier2_u8, &functionLocalControllerParameters->mutexes.heatExchangerPeltier2_mutex)) {
            ls_peltier2State_u8 = PELTIER_UNDEFINED;
        }
        //Peltier 3
        if (!readProtectedVariable(l_desiredPeltier3State_u8, &functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex)) {
            ls_peltier3State_u8 = PELTIER_UNDEFINED;
        }
        //Peltier 4
        if (!readProtectedVariable(l_desiredPeltier4State_u8, &functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex)) {
            ls_peltier4State_u8 = PELTIER_UNDEFINED;
        }
        drivePeltierCell(&ls_peltier1State_u8, &l_desiredPeltier1State_u8, &PeltierCell1, &ls_peltier1StateTransitionCounter_u16);
        drivePeltierCell(&ls_peltier2State_u8, &l_desiredPeltier2State_u8, &PeltierCell2, &ls_peltier2StateTransitionCounter_u16);
        drivePeltierCell(&ls_peltier3State_u8, &l_desiredPeltier3State_u8, &PeltierCell3, &ls_peltier3StateTransitionCounter_u16);
        drivePeltierCell(&ls_peltier4State_u8, &l_desiredPeltier4State_u8, &PeltierCell4, &ls_peltier4StateTransitionCounter_u16);

        vTaskDelay(500/portTICK_RATE_MS);
    }
}