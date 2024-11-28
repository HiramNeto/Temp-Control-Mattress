#include "sharedMemory.h"
void initializeMutexes(ControllerParameters* controllerParameters_p){
  controllerParameters_p->mutexes.heatExchangerFansPercent_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.heatExchangerHotSideTemp_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.heatExchangerInputPressure_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.heatExchangerOutputPressure_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.heatExchangerPeltier1_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.heatExchangerPeltier2_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.heatExchangerPeltier3_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.heatExchangerPeltier4_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.heatExchangerPump_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.heatExchangerWaterTemp_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.mattressInputPressure_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.mattressOutputPressure_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.mattressPumpPercent_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.mattressTempSetpoint_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.enableMattressTempCtrlSetpoint_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.mattressWaterOutputTemp_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.tankWaterTemp_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.heatExchangerBlockageEmergencyFlag_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.mattressBlockageEmergencyFlag_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.peltierTooHotEmergencyFlag_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.peltierTooColdEmergencyFlag_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.peltierWarningTempEmergencyFlag_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.tankWaterTempFailedEmergencyFlag_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.heatExchangerTempSensorUnavailableFlag_mutex = xSemaphoreCreateMutex();
  controllerParameters_p->mutexes.tempCtrlUnitStatus_mutex = xSemaphoreCreateMutex();
}

void initializeControllerVariables(ControllerParameters* controllerParameters_p){
  controllerParameters_p->sensors.heatExchangerHotSideTemp_f = UNDEFINED_FLOAT;
  controllerParameters_p->sensors.heatExchangerInputPressure_u32 = UNDEFINED_U32;
  controllerParameters_p->sensors.heatExchangerOutputPressure_u32 = UNDEFINED_U32;
  controllerParameters_p->sensors.heatExchangerWaterTemp_f = UNDEFINED_FLOAT;
  controllerParameters_p->sensors.mattressInputPressure_u32 = UNDEFINED_U32;
  controllerParameters_p->sensors.mattressOutputPressure_u32 = UNDEFINED_U32;
  controllerParameters_p->sensors.mattressWaterOutputTemp_f = UNDEFINED_FLOAT;
  controllerParameters_p->sensors.tankWaterTemp_f = UNDEFINED_FLOAT;
  
  controllerParameters_p->actuators.heatExchangerFansPercent_f = UNDEFINED_FLOAT;
  controllerParameters_p->actuators.heatExchangerPeltier1_u8 = UNDEFINED_U8;
  controllerParameters_p->actuators.heatExchangerPeltier2_u8 = UNDEFINED_U8;
  controllerParameters_p->actuators.heatExchangerPeltier3_u8 = UNDEFINED_U8;
  controllerParameters_p->actuators.heatExchangerPeltier4_u8 = UNDEFINED_U8;
  controllerParameters_p->actuators.heatExchangerPump_b = false;
  controllerParameters_p->actuators.mattressPumpPercent_f = UNDEFINED_FLOAT;
  
  controllerParameters_p->setpoints.mattressTemperature_f = UNDEFINED_FLOAT;
  controllerParameters_p->setpoints.enableMattressTemperatureControl_b = false;

  controllerParameters_p->emergencyFlags.heatExchangerBlockage_b = false;
  controllerParameters_p->emergencyFlags.mattressBlockage_b = false;
  controllerParameters_p->emergencyFlags.peltierTooHot_b = false;
  controllerParameters_p->emergencyFlags.heatExchangerTempSensorUnavailable_b = false;
  controllerParameters_p->emergencyFlags.peltierTooCold_b = false;
  controllerParameters_p->emergencyFlags.peltierWarningTemp_b = false;
  controllerParameters_p->emergencyFlags.tankTempFail_b = false;
  controllerParameters_p->statuses.tempCtrlUnitStatus_u8 = TEMPCTRL_OFF;
}