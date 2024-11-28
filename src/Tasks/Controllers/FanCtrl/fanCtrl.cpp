#include "fanCtrl.h"

uint16_t interpolatePwm(float deltaT, const uint16_t* deltaTTable, const uint16_t* deltaTPwmTable, size_t tableSize);
uint16_t feedForward(ControllerParameters *functionLocalControllerParameters);
uint16_t proportionalDeltaT(ControllerParameters *functionLocalControllerParameters);
uint16_t proportionalHeatSinkT(ControllerParameters *functionLocalControllerParameters);
uint16_t findMaxPwm(uint16_t l_pwm1_u16,uint16_t l_pwm2_u16,uint16_t l_pwm3_u16);

uint16_t interpolatePwm(float deltaT, const uint16_t* deltaTTable, const uint16_t* deltaTPwmTable, size_t tableSize) {
    // Verifica se o valor está abaixo ou igual ao primeiro valor da tabela
    if (deltaT <= deltaTTable[0]) {
        return deltaTPwmTable[0]; // Retorna o PWM mínimo
    }

    // Verifica se o valor está acima ou igual ao último valor da tabela
    if (deltaT >= deltaTTable[tableSize - 1]) {
        return deltaTPwmTable[tableSize - 1]; // Retorna o PWM máximo
    }

    // Procura os dois pontos entre os quais deltaT se encontra
    for (size_t i = 0; i < tableSize - 1; i++) {
        if (deltaTTable[i] <= deltaT && deltaT <= deltaTTable[i + 1]) {
            // Interpolação linear entre deltaTTable[i] e deltaTTable[i + 1]
            float proportion = (deltaT - deltaTTable[i]) / (deltaTTable[i + 1] - deltaTTable[i]);
            return deltaTPwmTable[i] + proportion * (deltaTPwmTable[i + 1] - deltaTPwmTable[i]);
        }
    }

    return 0; // Caso inesperado (não deveria chegar aqui)
}

uint16_t feedForwardTable[] = {100, 75, 50, 25, 0, 25, 50, 75, 100};

uint16_t feedForward(ControllerParameters *functionLocalControllerParameters){
    uint8_t l_peltierCell1State_u8;
    if (!readProtectedVariable(l_peltierCell1State_u8, &functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex)){
        return UNDEFINED_U16;
    }
    uint8_t l_peltierCell2State_u8;
    if (!readProtectedVariable(l_peltierCell2State_u8, &functionLocalControllerParameters->actuators.heatExchangerPeltier2_u8, &functionLocalControllerParameters->mutexes.heatExchangerPeltier2_mutex)){
        return UNDEFINED_U16;
    }
    uint8_t l_peltierCell3State_u8;
    if (!readProtectedVariable(l_peltierCell3State_u8, &functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex)){
        return UNDEFINED_U16;
    }
    uint8_t l_peltierCell4State_u8;
    if (!readProtectedVariable(l_peltierCell4State_u8, &functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex)){
        return UNDEFINED_U16;
    }
    
    if(
        l_peltierCell1State_u8 == PELTIER_HEATING_ON && 
        l_peltierCell2State_u8 == PELTIER_HEATING_ON && 
        l_peltierCell3State_u8 == PELTIER_HEATING_ON && 
        l_peltierCell4State_u8 == PELTIER_HEATING_ON
    ){
        return feedForwardTable[0];
    }else if(
        l_peltierCell1State_u8 == PELTIER_HEATING_ON && 
        l_peltierCell2State_u8 == PELTIER_HEATING_ON && 
        l_peltierCell3State_u8 == PELTIER_HEATING_ON && 
        l_peltierCell4State_u8 == PELTIER_OFF
    ){
        return feedForwardTable[1];
    }else if(
        l_peltierCell1State_u8 == PELTIER_HEATING_ON && 
        l_peltierCell2State_u8 == PELTIER_HEATING_ON && 
        l_peltierCell3State_u8 == PELTIER_OFF && 
        l_peltierCell4State_u8 == PELTIER_OFF
    ){
        return feedForwardTable[2];
    }else if(
        l_peltierCell1State_u8 == PELTIER_HEATING_ON && 
        l_peltierCell2State_u8 == PELTIER_OFF && 
        l_peltierCell3State_u8 == PELTIER_OFF && 
        l_peltierCell4State_u8 == PELTIER_OFF
    ){
        return feedForwardTable[3];
    }else if(
        l_peltierCell1State_u8 == PELTIER_OFF && 
        l_peltierCell2State_u8 == PELTIER_OFF && 
        l_peltierCell3State_u8 == PELTIER_OFF && 
        l_peltierCell4State_u8 == PELTIER_OFF
    ){
        return feedForwardTable[4];
    }else if(
        l_peltierCell1State_u8 == PELTIER_COOLING_ON && 
        l_peltierCell2State_u8 == PELTIER_OFF && 
        l_peltierCell3State_u8 == PELTIER_OFF && 
        l_peltierCell4State_u8 == PELTIER_OFF
    ){
        return feedForwardTable[5];
    }else if(
        l_peltierCell1State_u8 == PELTIER_COOLING_ON && 
        l_peltierCell2State_u8 == PELTIER_COOLING_ON && 
        l_peltierCell3State_u8 == PELTIER_OFF && 
        l_peltierCell4State_u8 == PELTIER_OFF
    ){
        return feedForwardTable[6];
    }else if(
        l_peltierCell1State_u8 == PELTIER_COOLING_ON && 
        l_peltierCell2State_u8 == PELTIER_COOLING_ON && 
        l_peltierCell3State_u8 == PELTIER_COOLING_ON && 
        l_peltierCell4State_u8 == PELTIER_OFF
    ){
        return feedForwardTable[7];
    }else if(
        l_peltierCell1State_u8 == PELTIER_COOLING_ON && 
        l_peltierCell2State_u8 == PELTIER_COOLING_ON && 
        l_peltierCell3State_u8 == PELTIER_COOLING_ON && 
        l_peltierCell4State_u8 == PELTIER_COOLING_ON
    ){
        return feedForwardTable[8];
    }else{
        return UNDEFINED_U16;
    }

}

uint16_t deltaTTable[]    = {0,5,10,15,20,25,30};
uint16_t deltaTPwmTable[] = {0,17,34,51,68,85,100};

uint16_t cooling_heatsinkTempTable[]       = {0,15,30,45,60,75,80};
uint16_t cooling_heatsinkTempPwmTable[] = {0,20,30,50,70,90,100};

uint16_t heating_heatsinkTempTable[]       = {0,5,10,15,20,25,30};
uint16_t heating_heatsinkTempPwmTable[] = {100,85,68,51,34,17,0};

uint16_t proportionalDeltaT(ControllerParameters *functionLocalControllerParameters){
    // y = y1 + (y2-y1) x [(x-x1) / (x2-x1)].
    float l_heatSinkTemp_f;
    if (!readProtectedVariable(l_heatSinkTemp_f, &functionLocalControllerParameters->sensors.heatExchangerHotSideTemp_f, &functionLocalControllerParameters->mutexes.heatExchangerHotSideTemp_mutex)){
        return UNDEFINED_U16;
    }
    float l_heatExchangerWaterTemp_f;
    if (!readProtectedVariable(l_heatExchangerWaterTemp_f, &functionLocalControllerParameters->sensors.heatExchangerWaterTemp_f, &functionLocalControllerParameters->mutexes.heatExchangerWaterTemp_mutex)){
        return UNDEFINED_U16;
    }
    float l_deltaT_f = fabs(l_heatSinkTemp_f-l_heatExchangerWaterTemp_f);
    size_t tableSize = sizeof(deltaTTable) / sizeof(deltaTTable[0]);
    
    return interpolatePwm(l_deltaT_f,deltaTTable,deltaTPwmTable,tableSize);
}

uint16_t proportionalHeatSinkT(ControllerParameters *functionLocalControllerParameters){
    float l_heatSinkTemp_f;
    uint16_t l_returnValue_u16;
    uint8_t l_tempCtrlStatus_u8 = TEMPCTRL_UNDEFINED;
    //Read necessary information for control strategy
    if ((!readProtectedVariable(l_heatSinkTemp_f, &functionLocalControllerParameters->sensors.heatExchangerHotSideTemp_f, &functionLocalControllerParameters->mutexes.heatExchangerHotSideTemp_mutex)) || (!readProtectedVariable(l_tempCtrlStatus_u8, &functionLocalControllerParameters->statuses.tempCtrlUnitStatus_u8, &functionLocalControllerParameters->mutexes.tempCtrlUnitStatus_mutex))){
        //return undefined if reading fails
        l_returnValue_u16 = UNDEFINED_U16;
    }
    else{
        //Check the status of temp. control unit: if it is heating
        if(l_tempCtrlStatus_u8>=TEMPCTRL_HEATING_1 && l_tempCtrlStatus_u8<=TEMPCTRL_HEATING_4){
            //ToDo: return something
            //heating_heatsinkTempTable & heating_heatsinkTempPwmTable
            size_t tableSize = sizeof(heating_heatsinkTempTable) / sizeof(heating_heatsinkTempTable[0]);
            return interpolatePwm(l_heatSinkTemp_f,heating_heatsinkTempTable,heating_heatsinkTempPwmTable,tableSize);
        }
        //Check the status of temp. control unit: if it is cooling
        else if(l_tempCtrlStatus_u8>=TEMPCTRL_COOLING_1 && l_tempCtrlStatus_u8<=TEMPCTRL_COOLING_4){
            //ToDo: return something
            //cooling_heatsinkTempTable & cooling_heatsinkTempPwmTable
            size_t tableSize = sizeof(cooling_heatsinkTempTable) / sizeof(cooling_heatsinkTempTable[0]);
            return interpolatePwm(l_heatSinkTemp_f,cooling_heatsinkTempTable,cooling_heatsinkTempPwmTable,tableSize);
        }
        //otherwise system is off, no need to drive fan
        else{
            l_returnValue_u16 = 0;
        }
    }
    
    return l_returnValue_u16;
}

uint16_t findMaxPwm(uint16_t l_pwm1_u16,uint16_t l_pwm2_u16,uint16_t l_pwm3_u16){
    uint16_t l_returnPwm_u16 = 0;
    if(l_pwm1_u16== UNDEFINED_U16 && l_pwm2_u16== UNDEFINED_U16 && l_pwm3_u16== UNDEFINED_U16){
        l_returnPwm_u16 = 100; //Max fan rpm for safety!
    }else{
        if(l_pwm1_u16!= UNDEFINED_U16 && l_pwm1_u16>l_returnPwm_u16){
        l_returnPwm_u16 = l_pwm1_u16;
        }
        if(l_pwm2_u16!= UNDEFINED_U16 && l_pwm2_u16>l_returnPwm_u16){
            l_returnPwm_u16 = l_pwm2_u16;
        }
        if(l_pwm3_u16!= UNDEFINED_U16 && l_pwm3_u16>l_returnPwm_u16){
            l_returnPwm_u16 = l_pwm3_u16;
        }
    }
    return l_returnPwm_u16;
}

uint16_t checkEmergencyConditions(ControllerParameters *functionLocalControllerParameters){
    bool peltierTooHot_b;
    bool peltierTooCold_b;
    bool heatExchangerTempSensorUnavailable_b;
    bool peltierWarningTemp_b;
    bool tankTempFail_b;

    uint16_t l_returnValue_u16;

    readProtectedVariable(peltierTooHot_b, &functionLocalControllerParameters->emergencyFlags.peltierTooHot_b, &functionLocalControllerParameters->mutexes.peltierTooHotEmergencyFlag_mutex);
    readProtectedVariable(peltierTooCold_b, &functionLocalControllerParameters->emergencyFlags.peltierTooCold_b, &functionLocalControllerParameters->mutexes.peltierTooColdEmergencyFlag_mutex);
    readProtectedVariable(heatExchangerTempSensorUnavailable_b, &functionLocalControllerParameters->emergencyFlags.heatExchangerTempSensorUnavailable_b, &functionLocalControllerParameters->mutexes.heatExchangerTempSensorUnavailableFlag_mutex);
    readProtectedVariable(peltierWarningTemp_b, &functionLocalControllerParameters->emergencyFlags.peltierWarningTemp_b, &functionLocalControllerParameters->mutexes.peltierWarningTempEmergencyFlag_mutex);
    readProtectedVariable(tankTempFail_b, &functionLocalControllerParameters->emergencyFlags.tankTempFail_b, &functionLocalControllerParameters->mutexes.tankWaterTempFailedEmergencyFlag_mutex);

    if(peltierTooHot_b || peltierTooCold_b || heatExchangerTempSensorUnavailable_b || peltierWarningTemp_b){
        l_returnValue_u16 = 100;
        char message[DEBUG_BUFFER_SIZE];
        snprintf(message, DEBUG_BUFFER_SIZE, "FAN turned 100% because of emergency protection");
        logDebugMessage(message, TASKID_FANCTRL);
    }else{
        l_returnValue_u16 = 0;
    }
}

void fanCtrlTask(void* functionInputParam){
    //Variables for the task
    ControllerParameters *functionLocalControllerParameters = (ControllerParameters*)functionInputParam;
    
    //Add code that will run only once here!

    //Loop
    while(1){
        char message[DEBUG_BUFFER_SIZE];

        //Loop task!
        //1) Feed-Forward (depending on the power applied to the peltier cells)
        uint16_t l_feedForwardPWM_u16 = feedForward(functionLocalControllerParameters);
        snprintf(message, DEBUG_BUFFER_SIZE, "FAN Feedforward PWM: %u",l_feedForwardPWM_u16);
        logDebugMessage(message, TASKID_FANCTRL);
        //2) Proportional to deltaT
        uint16_t l_deltaTPWM_u16 = proportionalDeltaT(functionLocalControllerParameters);
        snprintf(message, DEBUG_BUFFER_SIZE, "FAN deltaT PWM: %u",l_deltaTPWM_u16);
        logDebugMessage(message, TASKID_FANCTRL);
        //3) Proportional to heat exchanger temp (depending if the systems is cooling or heating the water)
        uint16_t l_heatSinkPWM_u16 = proportionalHeatSinkT(functionLocalControllerParameters);
        snprintf(message, DEBUG_BUFFER_SIZE, "FAN Heat Sink Temp. PWM: %u",l_heatSinkPWM_u16);
        logDebugMessage(message, TASKID_FANCTRL);

        //Apply the maximum value to the FANs
        uint16_t l_pwmToApply_u16 = findMaxPwm(l_feedForwardPWM_u16,l_deltaTPWM_u16,l_heatSinkPWM_u16);
        snprintf(message, DEBUG_BUFFER_SIZE, "PWM applied to FAN power mosfet: %u",l_pwmToApply_u16);
        logDebugMessage(message, TASKID_FANCTRL);

        if(!writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerFansPercent_f, (float)l_pwmToApply_u16, &functionLocalControllerParameters->mutexes.heatExchangerFansPercent_mutex)){
            snprintf(message, DEBUG_BUFFER_SIZE, "Cannot set fan ctrl pwm to the actuator");
            logDebugMessage(message, TASKID_FANCTRL);
        }

        vTaskDelay(1000/portTICK_RATE_MS);
    }

}