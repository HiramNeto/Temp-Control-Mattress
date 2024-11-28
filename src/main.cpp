#include <Arduino.h>
#include "./Constants/constants.h"
#include "./SharedMemory/sharedMemory.h"
#include "./Tasks/Debug/debug.h"
#include "./Tasks/Test/test.h"
#include "./Tasks/IO/DS18B20_TemperatureSensors/DS18B20_TemperatureSensors.h"
#include "./Tasks/IO/HX710B_PressureSensors/HX710B_PressureSensors.h"
#include "./Tasks/IO/PWM_Mosfets/PWM_Mosfets.h"
#include "./Tasks/IO/H_Bridge/H_Bridge.h"
#include "./Tasks/Controllers/TempCtrl/tempCtrl.h"
#include "./Tasks/Controllers/FanCtrl/fanCtrl.h"

/*IO Definition
  Outputs:
  *****************************************************
  Pump Control:
  1)Heat Exchanger Pump (ON/OFF - 1 pin)
  2)Mattress Pump (H-BRIDGE - 2 pin) 
  Fan Control:
  1)Heat Exchanger Fans (PWM - 1 pin)
  Peltier Control:
  1) Peltier 1 (FORWARD/REVERSE/OFF -> H-BRIDGE - 2 pin)
  2) Peltier 2 (FORWARD/REVERSE/OFF -> H-BRIDGE - 2 pin)
  3) Peltier 3 (FORWARD/REVERSE/OFF -> H-BRIDGE - 2 pin)
  4) Peltier 4 (FORWARD/REVERSE/OFF -> H-BRIDGE - 2 pin)
  *****************************************************
  
  Inputs:
  *****************************************************
  Pressure Sensors:
  1)Mattress Input Pressure (HX710B 2 Pin)
  2)Mattress Output Pressure (HX710B 2 Pin)
  3)Heat Exchanger Input Pressure (TBC) (HX710B 2 Pin);
  4)Heat Exchanger Output Pressure (TBC) (HX710B 2 Pin);
  Temperature Sensors:
  1)Tank Water Temp (onewire 1 Pin)
  2)Heat Exchanger Hot Side Temp (onewire 1 Pin)
  3)Heat Exchanger Water Temp (onewire 1 Pin)
  4)Mattress Water Output Temp (onewire 1 Pin)
  *****************************************************
*/

ControllerParameters controllerParameters;

void setup() {
  Serial.begin(115200);
  initializeMutexes(&controllerParameters);
  initializeControllerVariables(&controllerParameters);
  delay(100);
  xTaskCreatePinnedToCore(&print_task,"PRINT_TASK",12288,(void*)&controllerParameters,1,NULL,0);
  delay(500);
  initTemperatureSensors();
  initPowerMosfets();
  initHBridge();
  uint8_t pressureSensorInitResult = initPressureSensors();
  if(pressureSensorInitResult==0){
    Serial.println("Pressure Sensors Initialized Successfully!");
  }else{
    Serial.print("Error while initializing pressure sensors. Error code: ");
    Serial.println(pressureSensorInitResult);
  }
  delay(1000);
  xTaskCreatePinnedToCore(&temperatureSensorsTask,"TEMP_SENSORS_TASK",4096,(void*)&controllerParameters,1,NULL,0);
  xTaskCreatePinnedToCore(&pressureSensorsTask,"PRESS_SENSORS_TASK",4096,(void*)&controllerParameters,1,NULL,0);
  xTaskCreatePinnedToCore(&powerMosfetsTask,"PWR_MOSFETS_TASK",4096,(void*)&controllerParameters,1,NULL,0);
  xTaskCreatePinnedToCore(&hBridgeTask,"HBRIDGE_TASK",4096,(void*)&controllerParameters,1,NULL,0);
  xTaskCreatePinnedToCore(&tempCtrlTask,"TEMPCTRL_TASK",12288,(void*)&controllerParameters,1,NULL,0);
  xTaskCreatePinnedToCore(&fanCtrlTask,"FANCTRL_TASK",4096,(void*)&controllerParameters,1,NULL,0);
  //ToDo:Pumps Controls
  //xTaskCreatePinnedToCore(&test_function,"RANDOM_VALUE_TASK",4096,(void*)&controllerParameters,1,NULL,0);
}

void loop() {
  delay(1000);
}

