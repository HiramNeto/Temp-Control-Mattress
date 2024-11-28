#include "tempCtrl.h"

#define PELTIER_POWER_OFF 0
#define PELTIER_POWER_COOLING_LV1 1
#define PELTIER_POWER_COOLING_LV2 2
#define PELTIER_POWER_COOLING_LV3 3
#define PELTIER_POWER_COOLING_LV4 4
#define PELTIER_POWER_HEATING_LV1 5
#define PELTIER_POWER_HEATING_LV2 6
#define PELTIER_POWER_HEATING_LV3 7
#define PELTIER_POWER_HEATING_LV4 8

#define TEMPCTRL_RUN_EVERY_X_CYCLES 10

//Function definitions
void actuatePeltierCells(ControllerParameters *functionLocalControllerParameters, uint8_t l_desiredPower_u8);
void overheatingProtection(ControllerParameters* functionLocalControllerParameters);
void condensationProtection(ControllerParameters* functionLocalControllerParameters);
void tempControl(ControllerParameters* functionLocalControllerParameters);
void setupFuzzyController();
int mapDiscreteLevels(float saida) ;
void applyControl(int powerLevel, ControllerParameters *functionLocalControllerParameters);

void actuatePeltierCells(ControllerParameters *functionLocalControllerParameters, uint8_t l_desiredPower_u8){
    char message[DEBUG_BUFFER_SIZE];
        
    //Warning temp protection, cap power to 50% maximum...
    bool peltierWarningTemp_b;
    readProtectedVariable(peltierWarningTemp_b, &functionLocalControllerParameters->emergencyFlags.peltierWarningTemp_b, &functionLocalControllerParameters->mutexes.peltierWarningTempEmergencyFlag_mutex);
    
    switch(l_desiredPower_u8){
        default:
        case PELTIER_POWER_OFF:{
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier2_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier2_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex);
            writeProtectedVariable(functionLocalControllerParameters->statuses.tempCtrlUnitStatus_u8, (uint8_t)TEMPCTRL_OFF, &functionLocalControllerParameters->mutexes.tempCtrlUnitStatus_mutex);
        }break;
        case PELTIER_POWER_COOLING_LV1:{
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_COOLING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier2_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier2_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex);
            writeProtectedVariable(functionLocalControllerParameters->statuses.tempCtrlUnitStatus_u8, (uint8_t)TEMPCTRL_COOLING_1, &functionLocalControllerParameters->mutexes.tempCtrlUnitStatus_mutex);
        }break;
        case PELTIER_POWER_COOLING_LV2:{
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_COOLING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier2_u8, (uint8_t)PELTIER_COOLING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier2_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex);
            writeProtectedVariable(functionLocalControllerParameters->statuses.tempCtrlUnitStatus_u8, (uint8_t)TEMPCTRL_COOLING_2, &functionLocalControllerParameters->mutexes.tempCtrlUnitStatus_mutex);
        }break;
        case PELTIER_POWER_COOLING_LV3:{
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_COOLING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier2_u8, (uint8_t)PELTIER_COOLING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier2_mutex);
            //if warning temp is set, only allow 50% power on peltiers...
            if(!peltierWarningTemp_b){
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_COOLING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
            }else{
                snprintf(message, DEBUG_BUFFER_SIZE, "Cooling set to 75%, but system is capped to 50% because of temperature warning!");
                logDebugMessage(message, TASKID_TEMPCTRL);
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
            }
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex);
            writeProtectedVariable(functionLocalControllerParameters->statuses.tempCtrlUnitStatus_u8, (uint8_t)TEMPCTRL_COOLING_3, &functionLocalControllerParameters->mutexes.tempCtrlUnitStatus_mutex);
        }break;
        case PELTIER_POWER_COOLING_LV4:{
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_COOLING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier2_u8, (uint8_t)PELTIER_COOLING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier2_mutex);
            //if warning temp is set, only allow 50% power on peltiers...
            if(!peltierWarningTemp_b){
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_COOLING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, (uint8_t)PELTIER_COOLING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex);
            }else{
                snprintf(message, DEBUG_BUFFER_SIZE, "Cooling set to 100%, but system is capped to 50% because of temperature warning!");
                logDebugMessage(message, TASKID_TEMPCTRL);
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex);
            }
            writeProtectedVariable(functionLocalControllerParameters->statuses.tempCtrlUnitStatus_u8, (uint8_t)TEMPCTRL_COOLING_4, &functionLocalControllerParameters->mutexes.tempCtrlUnitStatus_mutex);
        }break;
        case PELTIER_POWER_HEATING_LV1:{
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_HEATING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier2_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier2_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex);
            writeProtectedVariable(functionLocalControllerParameters->statuses.tempCtrlUnitStatus_u8, (uint8_t)TEMPCTRL_HEATING_1, &functionLocalControllerParameters->mutexes.tempCtrlUnitStatus_mutex);
        }break;
        case PELTIER_POWER_HEATING_LV2:{
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_HEATING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier2_u8, (uint8_t)PELTIER_HEATING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier2_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex);
            writeProtectedVariable(functionLocalControllerParameters->statuses.tempCtrlUnitStatus_u8, (uint8_t)TEMPCTRL_HEATING_2, &functionLocalControllerParameters->mutexes.tempCtrlUnitStatus_mutex);
        }break;
        case PELTIER_POWER_HEATING_LV3:{
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_HEATING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier2_u8, (uint8_t)PELTIER_HEATING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier2_mutex);
            //if warning temp is set, only allow 50% power on peltiers...
            if(!peltierWarningTemp_b){
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_HEATING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
            }else{
                snprintf(message, DEBUG_BUFFER_SIZE, "Heating set to 75%, but system is capped to 50% because of temperature warning!");
                logDebugMessage(message, TASKID_TEMPCTRL);
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
            }
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex);
            writeProtectedVariable(functionLocalControllerParameters->statuses.tempCtrlUnitStatus_u8, (uint8_t)TEMPCTRL_HEATING_3, &functionLocalControllerParameters->mutexes.tempCtrlUnitStatus_mutex);
        }break;
        case PELTIER_POWER_HEATING_LV4:{
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_HEATING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
            writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier2_u8, (uint8_t)PELTIER_HEATING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier2_mutex);
            if(!peltierWarningTemp_b){
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_HEATING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, (uint8_t)PELTIER_HEATING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex);
            }else{
                snprintf(message, DEBUG_BUFFER_SIZE, "Heating set to 100%, but system is capped to 50% because of temperature warning!");
                logDebugMessage(message, TASKID_TEMPCTRL);
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier3_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier3_mutex);
                writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier4_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier4_mutex);
            }
            writeProtectedVariable(functionLocalControllerParameters->statuses.tempCtrlUnitStatus_u8, (uint8_t)TEMPCTRL_HEATING_4, &functionLocalControllerParameters->mutexes.tempCtrlUnitStatus_mutex);
        }break;
    }
}

void overheatingProtection(ControllerParameters* functionLocalControllerParameters){
    char message[DEBUG_BUFFER_SIZE];

    //Monitor Peltier hot side temperature - Overheating protection
    float l_heatExchangerHotSideTemp_f;
    if (readProtectedVariable(l_heatExchangerHotSideTemp_f, &functionLocalControllerParameters->sensors.heatExchangerHotSideTemp_f, &functionLocalControllerParameters->mutexes.heatExchangerHotSideTemp_mutex)) {
        if(l_heatExchangerHotSideTemp_f == UNDEFINED_FLOAT){
            snprintf(message, DEBUG_BUFFER_SIZE, "Heat exchanger hot side temperature undefined! Overheating protection function interrupted.\n");
            logDebugMessage(message, TASKID_TEMPCTRL);
            writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.heatExchangerTempSensorUnavailable_b, true, &functionLocalControllerParameters->mutexes.heatExchangerTempSensorUnavailableFlag_mutex);
            writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierTooHot_b, false, &functionLocalControllerParameters->mutexes.peltierTooHotEmergencyFlag_mutex);
            writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierWarningTemp_b, false, &functionLocalControllerParameters->mutexes.peltierWarningTempEmergencyFlag_mutex);    
            return;
        }else{
            writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.heatExchangerTempSensorUnavailable_b, false, &functionLocalControllerParameters->mutexes.heatExchangerTempSensorUnavailableFlag_mutex);
        }

        if(l_heatExchangerHotSideTemp_f>PELTIER_HOTSIDE_MAXTEMP){
            snprintf(message, DEBUG_BUFFER_SIZE, "Heat exchanger temperature: %f.\n",l_heatExchangerHotSideTemp_f);
            logDebugMessage(message, TASKID_TEMPCTRL);
            snprintf(message, DEBUG_BUFFER_SIZE, "Heat exchanger HotSide Temperature bigger than limit! Overheating protection!.\n");
            logDebugMessage(message, TASKID_TEMPCTRL);
            //Overheat protection: too hot true, warning temp false
            writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierTooHot_b, true, &functionLocalControllerParameters->mutexes.peltierTooHotEmergencyFlag_mutex);
            writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierWarningTemp_b, false, &functionLocalControllerParameters->mutexes.peltierWarningTempEmergencyFlag_mutex);    
        }else{
            //too hot false
            writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierTooHot_b, false, &functionLocalControllerParameters->mutexes.peltierTooHotEmergencyFlag_mutex);
            //Check warning temp
            if(l_heatExchangerHotSideTemp_f>PELTIER_HOTSIDE_WARNINGTEMP){
                //Overheat warning protection: warning temp true
                snprintf(message, DEBUG_BUFFER_SIZE, "Heat exchanger HotSide Temperature bigger than warning threshold! Warning!.\n");
                logDebugMessage(message, TASKID_TEMPCTRL);
                writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierWarningTemp_b, true, &functionLocalControllerParameters->mutexes.peltierWarningTempEmergencyFlag_mutex);    
            }else{
                //warning temp false
                writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierWarningTemp_b, false, &functionLocalControllerParameters->mutexes.peltierWarningTempEmergencyFlag_mutex);    
            }
        }
    }else{
        char message[DEBUG_BUFFER_SIZE];
        snprintf(message, DEBUG_BUFFER_SIZE, "Error while trying to access heat exchanger temp value. Entering emergency mode for precaution...\n");
        logDebugMessage(message, TASKID_TEMPCTRL);
        
        writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierTooHot_b, true, &functionLocalControllerParameters->mutexes.peltierTooHotEmergencyFlag_mutex);
        writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierWarningTemp_b, false, &functionLocalControllerParameters->mutexes.peltierWarningTempEmergencyFlag_mutex);
    }
}

void condensationProtection(ControllerParameters* functionLocalControllerParameters){
    //Monitor Peltier water temperature - Condensation protection
    float l_heatExchangerWaterTemp_f;
    if (readProtectedVariable(l_heatExchangerWaterTemp_f, &functionLocalControllerParameters->sensors.heatExchangerWaterTemp_f, &functionLocalControllerParameters->mutexes.heatExchangerWaterTemp_mutex)) {
        if(l_heatExchangerWaterTemp_f<PELTIER_WATER_CONDENSATIONTEMP){
            //Condensation protection: too cold true
            writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierTooCold_b, true, &functionLocalControllerParameters->mutexes.peltierTooColdEmergencyFlag_mutex);
        }else{
            writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierTooCold_b, false, &functionLocalControllerParameters->mutexes.peltierTooColdEmergencyFlag_mutex);
        }
    }else{
        char message[DEBUG_BUFFER_SIZE];
        snprintf(message, DEBUG_BUFFER_SIZE, "Error while trying to access heat exchanger water temp value. Entering emergency mode for precaution...");
        logDebugMessage(message, TASKID_TEMPCTRL);
        writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.peltierTooCold_b, true, &functionLocalControllerParameters->mutexes.peltierTooColdEmergencyFlag_mutex);
    }
}

//Fuzzy controller instance
Fuzzy* fuzzy = new Fuzzy();

//Allowed power levels
int powerLevels[] = {-100, -75, -50, -25, 0, 25, 50, 75, 100};

void setupFuzzyController(){
    // Definição das variáveis fuzzy
  // Entrada 1: Erro
  FuzzyInput* erroFuzzy = new FuzzyInput(1);

  // Conjuntos fuzzy para 'Erro'
  FuzzySet* ng = new FuzzySet(-20, -20, -15, -10); // Negativo Grande (NG)
  FuzzySet* nm = new FuzzySet(-15, -10, -10, -5);  // Negativo Médio (NM)
  FuzzySet* ze = new FuzzySet(-10, -5, 0, 5);      // Zero (ZE)
  FuzzySet* pm = new FuzzySet(5, 10, 10, 15);      // Positivo Médio (PM)
  FuzzySet* pg = new FuzzySet(10, 15, 20, 20);     // Positivo Grande (PG)

  erroFuzzy->addFuzzySet(ng);
  erroFuzzy->addFuzzySet(nm);
  erroFuzzy->addFuzzySet(ze);
  erroFuzzy->addFuzzySet(pm);
  erroFuzzy->addFuzzySet(pg);

  fuzzy->addFuzzyInput(erroFuzzy);

  // Entrada 2: DeltaErro
  FuzzyInput* deltaErroFuzzy = new FuzzyInput(2);

  // Conjuntos fuzzy para 'DeltaErro'
  FuzzySet* ne = new FuzzySet(-10, -10, -5, 0); // Negativo (N)
  FuzzySet* ze_d = new FuzzySet(-5, 0, 0, 5);   // Zero (ZE)
  FuzzySet* po = new FuzzySet(0, 5, 10, 10);    // Positivo (P)

  deltaErroFuzzy->addFuzzySet(ne);
  deltaErroFuzzy->addFuzzySet(ze_d);
  deltaErroFuzzy->addFuzzySet(po);

  fuzzy->addFuzzyInput(deltaErroFuzzy);

  // Saída: Nível de Potência
  FuzzyOutput* potenciaFuzzy = new FuzzyOutput(1);

  // Conjuntos fuzzy para 'Nível de Potência'
  FuzzySet* mf = new FuzzySet(-100, -100, -100, -75); // Muito Frio (MF)
  FuzzySet* fr = new FuzzySet(-75, -50, -50, -25);    // Frio (F)
  FuzzySet* nu = new FuzzySet(-25, 0, 0, 25);         // Neutro (N)
  FuzzySet* qu = new FuzzySet(25, 50, 50, 75);        // Quente (Q)
  FuzzySet* mq = new FuzzySet(75, 100, 100, 100);     // Muito Quente (MQ)

  potenciaFuzzy->addFuzzySet(mf);
  potenciaFuzzy->addFuzzySet(fr);
  potenciaFuzzy->addFuzzySet(nu);
  potenciaFuzzy->addFuzzySet(qu);
  potenciaFuzzy->addFuzzySet(mq);

  fuzzy->addFuzzyOutput(potenciaFuzzy);

  // Definição das regras fuzzy
  // Exemplo de regra:
  // Se Erro é NG e DeltaErro é N, então Potência é MQ

  // Antecedente
  FuzzyRuleAntecedent* ifErroNGAndDeltaErroN = new FuzzyRuleAntecedent();
  ifErroNGAndDeltaErroN->joinWithAND(ng, ne);

  // Consequente
  FuzzyRuleConsequent* thenPotenciaMQ = new FuzzyRuleConsequent();
  thenPotenciaMQ->addOutput(mq);

  // Regra
  FuzzyRule* fuzzyRule1 = new FuzzyRule(1, ifErroNGAndDeltaErroN, thenPotenciaMQ);
  fuzzy->addFuzzyRule(fuzzyRule1);
}

void tempControl(ControllerParameters* functionLocalControllerParameters){
    // verify emergency flags
    bool peltierTooHot_b;
    bool peltierTooCold_b;
    bool heatExchangerTempSensorUnavailable_b;
    readProtectedVariable(peltierTooHot_b, &functionLocalControllerParameters->emergencyFlags.peltierTooHot_b, &functionLocalControllerParameters->mutexes.peltierTooHotEmergencyFlag_mutex);
    readProtectedVariable(peltierTooCold_b, &functionLocalControllerParameters->emergencyFlags.peltierTooCold_b, &functionLocalControllerParameters->mutexes.peltierTooColdEmergencyFlag_mutex);
    readProtectedVariable(heatExchangerTempSensorUnavailable_b, &functionLocalControllerParameters->emergencyFlags.heatExchangerTempSensorUnavailable_b, &functionLocalControllerParameters->mutexes.heatExchangerTempSensorUnavailableFlag_mutex);
    
    
    if(peltierTooHot_b || peltierTooCold_b || heatExchangerTempSensorUnavailable_b){
        //shut down Peltier modules
        actuatePeltierCells(functionLocalControllerParameters, PELTIER_POWER_OFF);
        //log error message
        char message[DEBUG_BUFFER_SIZE];
        if(peltierTooHot_b){
            snprintf(message, DEBUG_BUFFER_SIZE, "Emergency condition (peltier too hot) detected. Peltier modules are turned off.\n");
            logDebugMessage(message, TASKID_TEMPCTRL);
        }
        if(peltierTooCold_b){
            snprintf(message, DEBUG_BUFFER_SIZE, "Emergency condition (peltier too cold) detected. Peltier modules are turned off.\n");
            logDebugMessage(message, TASKID_TEMPCTRL);
        }
        if(heatExchangerTempSensorUnavailable_b){
            snprintf(message, DEBUG_BUFFER_SIZE, "Emergency condition (heat exchanger temp unavailable) detected. Peltier modules are turned off.\n");
            logDebugMessage(message, TASKID_TEMPCTRL);
        }
        //return function, interrupt temp controller
        return;
    }
    
    
    float l_error_f = 0.0;
    float l_deltaError_f = 0.0;
    static float ls_previous_error_f = 0.0;
        
    //Controls tank temperature for a given setpoint
    float l_tankWaterTemp_f;
    float l_tankWaterTempSetpoint_f;
    bool l_controllerShouldRun_b = true;
    if (readProtectedVariable(l_tankWaterTemp_f, &functionLocalControllerParameters->sensors.tankWaterTemp_f, &functionLocalControllerParameters->mutexes.tankWaterTemp_mutex)) {
        writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.tankTempFail_b, false, &functionLocalControllerParameters->mutexes.tankWaterTempFailedEmergencyFlag_mutex);
        if (readProtectedVariable(l_tankWaterTempSetpoint_f, &functionLocalControllerParameters->setpoints.mattressTemperature_f, &functionLocalControllerParameters->mutexes.tankWaterTemp_mutex)) {
            if (readProtectedVariable(l_tankWaterTempSetpoint_f, &functionLocalControllerParameters->setpoints.mattressTemperature_f, &functionLocalControllerParameters->mutexes.tankWaterTemp_mutex)) {
                if(l_tankWaterTemp_f==UNDEFINED_FLOAT || l_tankWaterTempSetpoint_f==UNDEFINED_FLOAT){
                    l_controllerShouldRun_b = false;
                    if(l_tankWaterTemp_f==UNDEFINED_FLOAT){
                        char message[DEBUG_BUFFER_SIZE];
                        snprintf(message, DEBUG_BUFFER_SIZE, "Error: The water tank temperature value is undefined...");
                        logDebugMessage(message, TASKID_TEMPCTRL);
                    }
                    if(l_tankWaterTempSetpoint_f==UNDEFINED_FLOAT){
                        char message[DEBUG_BUFFER_SIZE];
                        snprintf(message, DEBUG_BUFFER_SIZE, "Error: The water tank temp setpoint value is undefined...");
                        logDebugMessage(message, TASKID_TEMPCTRL);
                    }                    
                }else{
                    char message[DEBUG_BUFFER_SIZE];
                    snprintf(message, DEBUG_BUFFER_SIZE, "The current water tank temp value is %.2f, and the setpoint value is %.2f\n", l_tankWaterTemp_f, l_tankWaterTempSetpoint_f);
                    logDebugMessage(message, TASKID_TEMPCTRL);
                }
            }else{
                l_controllerShouldRun_b = false;
                
                char message[DEBUG_BUFFER_SIZE];
                snprintf(message, DEBUG_BUFFER_SIZE, "Error while trying to access the activation switch setpoint value...\n");
                logDebugMessage(message, TASKID_TEMPCTRL);
            }
        }else{
            l_controllerShouldRun_b = false;
            
            char message[DEBUG_BUFFER_SIZE];
            snprintf(message, DEBUG_BUFFER_SIZE, "Error while trying to access the mattress temperature setpoint value...\n");
            logDebugMessage(message, TASKID_TEMPCTRL);
        }
    }else{
        char message[DEBUG_BUFFER_SIZE];
        snprintf(message, DEBUG_BUFFER_SIZE, "Error while trying to access the water tank temperature! Entering emergency mode for precaution!\n");
        logDebugMessage(message, TASKID_TEMPCTRL);
        
        l_controllerShouldRun_b = false;
        writeProtectedVariable(functionLocalControllerParameters->emergencyFlags.tankTempFail_b, true, &functionLocalControllerParameters->mutexes.tankWaterTempFailedEmergencyFlag_mutex);
    }
    if(l_controllerShouldRun_b){

        //Error and error deviation calculation
        l_error_f = l_tankWaterTempSetpoint_f - l_tankWaterTemp_f;
        l_deltaError_f = l_error_f - ls_previous_error_f;
        //Previous Error update
        ls_previous_error_f = l_error_f;

        //Fuzzy input definitions
        fuzzy->setInput(1, l_error_f);
        fuzzy->setInput(2, l_deltaError_f);

        //Fuzzy Processing
        fuzzy->fuzzify();

        //Get fuzzy output
        float fuzzyOutput = fuzzy->defuzzify(1);

        //Map to discrete levels
        int powerLevel = mapDiscreteLevels(fuzzyOutput);

        //Apply fuzzy control to the actuator
        applyControl(powerLevel,functionLocalControllerParameters);

    }else{
        //Disable all peltier outputs
        actuatePeltierCells(functionLocalControllerParameters,PELTIER_POWER_OFF);
    }
}

//Function to map the fuzzy putput to actuator discrete levels
int mapDiscreteLevels(float output) {
  int nextLevel = powerLevels[0];
  float smallerDifference = abs(output - powerLevels[0]);
  for (int i = 1; i < sizeof(powerLevels) / sizeof(powerLevels[0]); i++) {
    float difference = abs(output - powerLevels[i]);
    if (difference < smallerDifference) {
      smallerDifference = difference;
      nextLevel = powerLevels[i];
    }
  }
  return nextLevel;
}

//Function to apply control to the actuator
void applyControl(int powerLevel, ControllerParameters *functionLocalControllerParameters) {
  switch(powerLevel){
    default:
    case 0:{
        actuatePeltierCells(functionLocalControllerParameters,PELTIER_POWER_OFF);
    }break;
    case -100:{
        actuatePeltierCells(functionLocalControllerParameters,PELTIER_POWER_COOLING_LV4);
    }break;
    case -75:{
        actuatePeltierCells(functionLocalControllerParameters,PELTIER_POWER_COOLING_LV3);
    }break;
    case -50:{
        actuatePeltierCells(functionLocalControllerParameters,PELTIER_POWER_COOLING_LV2);
    }break;
    case -25:{
        actuatePeltierCells(functionLocalControllerParameters,PELTIER_POWER_COOLING_LV1);
    }break;
    case 25:{
        actuatePeltierCells(functionLocalControllerParameters,PELTIER_POWER_HEATING_LV1);
    }break;
    case 50:{
        actuatePeltierCells(functionLocalControllerParameters,PELTIER_POWER_HEATING_LV2);
    }break;
    case 75:{
        actuatePeltierCells(functionLocalControllerParameters,PELTIER_POWER_HEATING_LV3);
    }break;
    case 100:{
        actuatePeltierCells(functionLocalControllerParameters,PELTIER_POWER_HEATING_LV4);
    }break;
  }
}


void tempCtrlTask(void* functionInputParam){
    static uint8_t ls_tempControlRunCounter_u8;
    ControllerParameters *functionLocalControllerParameters = (ControllerParameters*)functionInputParam;
    
    //Add code that will run only once here!
    //Setup Fuzzy controller
    setupFuzzyController();

    while(1){
        //Loop task!
        //OverHeating Peltier Protection
        overheatingProtection(functionLocalControllerParameters);
        //Condensation heatexchanger protection
        condensationProtection(functionLocalControllerParameters);
        if(ls_tempControlRunCounter_u8==0){
            //Temperature control -> Fuzzy controller logic
            tempControl(functionLocalControllerParameters);
        }
        //increment counter
        ls_tempControlRunCounter_u8=(ls_tempControlRunCounter_u8<TEMPCTRL_RUN_EVERY_X_CYCLES-1)?ls_tempControlRunCounter_u8+1:0;

        vTaskDelay(1000/portTICK_RATE_MS);
    }
}