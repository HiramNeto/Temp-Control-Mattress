#include "HX710B_PressureSensors.h"

Pressure_Sensors pressureSensors{
    {
        {MATTRESS_INPUT_PRESSURE_ID, PRESSURE_SENSOR_1_SCKPIN, PRESSURE_SENSOR_1_SDIPIN},
        {MATTRESS_OUTPUT_PRESSURE_ID, PRESSURE_SENSOR_2_SCKPIN, PRESSURE_SENSOR_2_SDIPIN},
        {HEAT_EXCHANG_INPUT_PRESSURE_ID, PRESSURE_SENSOR_3_SCKPIN, PRESSURE_SENSOR_3_SDIPIN},
        {HEAT_EXCHANG_OUTPUT_PRESSURE_ID, PRESSURE_SENSOR_4_SCKPIN, PRESSURE_SENSOR_4_SDIPIN}
    }
};

uint8_t initPressureSensors(){
    uint8_t l_returnValue_u8 = 0;
    if (!pressureSensors.PressureSensorsArray[0].pressureSensor.init()){ 
      char message[DEBUG_BUFFER_SIZE];
      snprintf(message, DEBUG_BUFFER_SIZE, "Pressure sensor 1 not found !\n");
      logDebugMessage(message, TASKID_PRESSURESENSORS);
      l_returnValue_u8 += 1;
    }
    if (!pressureSensors.PressureSensorsArray[1].pressureSensor.init()){ 
      char message[DEBUG_BUFFER_SIZE];
      snprintf(message, DEBUG_BUFFER_SIZE, "Pressure sensor 2 not found !\n");
      logDebugMessage(message, TASKID_PRESSURESENSORS);
      l_returnValue_u8 += 2;
    }
    if (!pressureSensors.PressureSensorsArray[2].pressureSensor.init()){ 
      char message[DEBUG_BUFFER_SIZE];
      snprintf(message, DEBUG_BUFFER_SIZE, "Pressure sensor 3 not found !\n");
      logDebugMessage(message, TASKID_PRESSURESENSORS);
      l_returnValue_u8 += 4;
    }
    if (!pressureSensors.PressureSensorsArray[3].pressureSensor.init()){ 
      char message[DEBUG_BUFFER_SIZE];
      snprintf(message, DEBUG_BUFFER_SIZE, "Pressure sensor 4 not found !\n");
      logDebugMessage(message, TASKID_PRESSURESENSORS);
      l_returnValue_u8 += 8;
    }
    return l_returnValue_u8;
}

void pressureSensorsTask(void* functionInputParam){
  ControllerParameters *functionLocalControllerParameters = (ControllerParameters*)functionInputParam;
    
  while(1){
    uint32_t data_raw = 0;
    //Read Mattress Input Pressure
    uint8_t pressureSensorReadErrorCode = pressureSensors.PressureSensorsArray[0].pressureSensor.read(&data_raw, 1000UL);
    if ( pressureSensorReadErrorCode != HX710B_OK ){
      data_raw = UNDEFINED_U32;
    }
    writeProtectedVariable(functionLocalControllerParameters->sensors.mattressInputPressure_u32, data_raw, &functionLocalControllerParameters->mutexes.mattressInputPressure_mutex);

    //Read Mattress Output Pressure
    pressureSensorReadErrorCode = pressureSensors.PressureSensorsArray[1].pressureSensor.read(&data_raw, 1000UL);
    if ( pressureSensorReadErrorCode != HX710B_OK ){
      data_raw = UNDEFINED_U32;
    }
    writeProtectedVariable(functionLocalControllerParameters->sensors.mattressOutputPressure_u32, data_raw, &functionLocalControllerParameters->mutexes.mattressOutputPressure_mutex);

    //Read Heat Exchanger Input Pressure
    pressureSensorReadErrorCode = pressureSensors.PressureSensorsArray[2].pressureSensor.read(&data_raw, 1000UL);
    if ( pressureSensorReadErrorCode != HX710B_OK ){
      data_raw = UNDEFINED_U32;
    }
    writeProtectedVariable(functionLocalControllerParameters->sensors.heatExchangerInputPressure_u32, data_raw, &functionLocalControllerParameters->mutexes.heatExchangerInputPressure_mutex);

    //Read Heat Exchanger Output Pressure
    pressureSensorReadErrorCode = pressureSensors.PressureSensorsArray[3].pressureSensor.read(&data_raw, 1000UL);
    if ( pressureSensorReadErrorCode != HX710B_OK ){
      data_raw = UNDEFINED_U32;
    }
    writeProtectedVariable(functionLocalControllerParameters->sensors.heatExchangerOutputPressure_u32, data_raw, &functionLocalControllerParameters->mutexes.heatExchangerOutputPressure_mutex);
    
    vTaskDelay(2000/portTICK_RATE_MS);
  }
}