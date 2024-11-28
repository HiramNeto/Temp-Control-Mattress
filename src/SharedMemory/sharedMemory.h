#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../Constants/constants.h"

struct Sensors{
  //Temperature Sensors
  float tankWaterTemp_f;
  float heatExchangerWaterTemp_f;
  float heatExchangerHotSideTemp_f;
  float mattressWaterOutputTemp_f; //ToDo: not used ANYWHERE! Is this really needed????
  //Pressure Sensors
  uint32_t mattressInputPressure_u32;
  uint32_t mattressOutputPressure_u32;
  uint32_t heatExchangerInputPressure_u32;
  uint32_t heatExchangerOutputPressure_u32;
};

struct Actuators{
  //PWM Control
  float heatExchangerFansPercent_f;
  float mattressPumpPercent_f;
  //ON/OFF Control
  bool heatExchangerPump_b;
  //FORWARD/REVERSE/OFF Control
  uint8_t heatExchangerPeltier1_u8;
  uint8_t heatExchangerPeltier2_u8;
  uint8_t heatExchangerPeltier3_u8;
  uint8_t heatExchangerPeltier4_u8;
};

struct Setpoints{
  //Temperature Setpoints
  float mattressTemperature_f;
  boolean enableMattressTemperatureControl_b;
};

struct EmergencyFlags{
  boolean peltierWarningTemp_b;
  boolean heatExchangerTempSensorUnavailable_b;
  boolean peltierTooHot_b;
  boolean peltierTooCold_b;
  boolean heatExchangerBlockage_b;
  boolean mattressBlockage_b;
  boolean tankTempFail_b;
};

struct Statuses{
  uint8_t tempCtrlUnitStatus_u8;
};

struct SharedVariablesAccessControls{
  //Sensors Mutex
  SemaphoreHandle_t tankWaterTemp_mutex = NULL;
  SemaphoreHandle_t heatExchangerWaterTemp_mutex = NULL;
  SemaphoreHandle_t heatExchangerHotSideTemp_mutex = NULL;
  SemaphoreHandle_t mattressWaterOutputTemp_mutex = NULL;
  SemaphoreHandle_t mattressInputPressure_mutex = NULL;
  SemaphoreHandle_t mattressOutputPressure_mutex = NULL;
  SemaphoreHandle_t heatExchangerInputPressure_mutex = NULL;
  SemaphoreHandle_t heatExchangerOutputPressure_mutex = NULL;
  //Actuators Mutex
  SemaphoreHandle_t heatExchangerFansPercent_mutex = NULL;
  SemaphoreHandle_t heatExchangerPump_mutex = NULL;
  SemaphoreHandle_t mattressPumpPercent_mutex = NULL;
  SemaphoreHandle_t heatExchangerPeltier1_mutex = NULL;
  SemaphoreHandle_t heatExchangerPeltier2_mutex = NULL;
  SemaphoreHandle_t heatExchangerPeltier3_mutex = NULL;
  SemaphoreHandle_t heatExchangerPeltier4_mutex = NULL;
  //Setpoints Mutex
  SemaphoreHandle_t mattressTempSetpoint_mutex = NULL;
  SemaphoreHandle_t enableMattressTempCtrlSetpoint_mutex = NULL;
  //Emergency Flags Mutex
  SemaphoreHandle_t peltierWarningTempEmergencyFlag_mutex = NULL;
  SemaphoreHandle_t peltierTooHotEmergencyFlag_mutex = NULL;
  SemaphoreHandle_t peltierTooColdEmergencyFlag_mutex = NULL;
  SemaphoreHandle_t heatExchangerBlockageEmergencyFlag_mutex = NULL;
  SemaphoreHandle_t mattressBlockageEmergencyFlag_mutex = NULL;
  SemaphoreHandle_t tankWaterTempFailedEmergencyFlag_mutex = NULL;
  SemaphoreHandle_t heatExchangerTempSensorUnavailableFlag_mutex = NULL;
  //Statuses Mutex
  SemaphoreHandle_t tempCtrlUnitStatus_mutex = NULL;
};

struct ControllerParameters{
  Sensors sensors;
  Actuators actuators;
  Setpoints setpoints;
  EmergencyFlags emergencyFlags;
  SharedVariablesAccessControls mutexes;
  Statuses statuses;
};

template<typename T>
bool readProtectedVariable(T& outValue, const T* variable, SemaphoreHandle_t* mutex) {
    if (xSemaphoreTake(*mutex, portMAX_DELAY)){
        outValue = *variable; // Faz a leitura da variável
        xSemaphoreGive(*mutex);
        return true;
    } else {
        // Falha ao adquirir o mutex
        return false;
    }
}

template<typename T>
bool writeProtectedVariable(T& variable, const T newValue, SemaphoreHandle_t* mutex) {
    if (xSemaphoreTake(*mutex, portMAX_DELAY)) {
        variable = newValue; // Escreve o novo valor na variável
        xSemaphoreGive(*mutex);
        return true;
    } else {
        // Falha ao adquirir o mutex
        return false;
    }
}

void initializeMutexes(ControllerParameters* controllerParameters_p);
void initializeControllerVariables(ControllerParameters* controllerParameters_p);

#endif