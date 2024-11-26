#include "PWM_Mosfets.h"

PowerMosfets RecirculatePumpMosfet(MOSFETCTRL_PIN_RECIRCULATEPUMP);
PowerMosfets MattressPumpMosfet(MOSFETCTRL_PIN_MATTRESSPUMP);
PowerMosfets PeltierFansMosfet(MOSFETCTRL_PIN_PELTIERFANS);

void initPowerMosfets(){
    RecirculatePumpMosfet.init();
    MattressPumpMosfet.init();
    PeltierFansMosfet.init();
}

void powerMosfetsTask(void* functionInputParam){
    ControllerParameters *functionLocalControllerParameters = (ControllerParameters*)functionInputParam;
    while(1){
        float pwmPercent;
        bool onOffValue;
        uint8_t pwmValue;
        //Fans
        if (readProtectedVariable(pwmPercent, &functionLocalControllerParameters->actuators.heatExchangerFansPercent_f, &functionLocalControllerParameters->mutexes.heatExchangerFansPercent_mutex)) {
            if(pwmPercent != UNDEFINED_FLOAT){
                if(pwmValue>100.0){
                    pwmValue=100.0;
                }else if(pwmValue<0.0){
                    pwmValue=0.0;
                }
                pwmValue = (uint8_t)((pwmPercent / 100.0) * 255.0);
                PeltierFansMosfet.setDutyCycle(pwmValue);
            }else{
                PeltierFansMosfet.setDutyCycle(0);
            }
            
        }else{
            char message[DEBUG_BUFFER_SIZE];
            snprintf(message, DEBUG_BUFFER_SIZE, "Error while trying to access the fan pwm value!\n");
            logDebugMessage(message, TASKID_PWMMOSFETS);
            PeltierFansMosfet.setDutyCycle(255);
        }
        //Mattress Pump
        if (readProtectedVariable(pwmPercent, &functionLocalControllerParameters->actuators.mattressPumpPercent_f, &functionLocalControllerParameters->mutexes.mattressPumpPercent_mutex)) {
            if(pwmPercent != UNDEFINED_FLOAT){
                if(pwmValue>100.0){
                    pwmValue=100.0;
                }else if(pwmValue<0.0){
                    pwmValue=0.0;
                }
                pwmValue = (uint8_t)((pwmPercent / 100.0) * 255.0);
                MattressPumpMosfet.setDutyCycle(pwmValue);
            }else{
                MattressPumpMosfet.setDutyCycle(0);
            }
            
        }else{
            char message[DEBUG_BUFFER_SIZE];
            snprintf(message, DEBUG_BUFFER_SIZE, "Error while trying to access the matress pump pwm value!\n");
            logDebugMessage(message, TASKID_PWMMOSFETS);
            MattressPumpMosfet.setDutyCycle(255);
        }
        //Recirculation Pump
        if (readProtectedVariable(onOffValue, &functionLocalControllerParameters->actuators.heatExchangerPump_b, &functionLocalControllerParameters->mutexes.heatExchangerPump_mutex)) {
            RecirculatePumpMosfet.setDutyCycle((onOffValue)?255:0);
        }else{
            char message[DEBUG_BUFFER_SIZE];
            snprintf(message, DEBUG_BUFFER_SIZE, "Error while trying to access the recirculation pump on/off value!\n");
            logDebugMessage(message, TASKID_PWMMOSFETS);
            RecirculatePumpMosfet.setDutyCycle(255);
        }
        vTaskDelay(500/portTICK_RATE_MS);
    }

}