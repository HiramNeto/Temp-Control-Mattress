#include "DS18B20_TemperatureSensors.h"

Temperature_Sensors CooledMattress_TemperatureSensors;

// Setup a oneWire instance to communicate with a OneWire device
OneWire oneWire(TEMPSENSORS_CONNECTIONPIN);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void returnDeviceAddressFromName(DeviceAddress* returnValuePointer, uint8_t DeviceNameIndex);

void initTemperatureSensors(){
    sensors.begin();
    CooledMattress_TemperatureSensors.TemperatureSensors[0].deviceNameIndex_u8 = TANK_WATER_TEMP;
    CooledMattress_TemperatureSensors.TemperatureSensors[0].sensorAddress[0] = 0x9F;
    CooledMattress_TemperatureSensors.TemperatureSensors[0].sensorAddress[1] = 0x9D; 
    CooledMattress_TemperatureSensors.TemperatureSensors[0].sensorAddress[2] = 0x87; 
    CooledMattress_TemperatureSensors.TemperatureSensors[0].sensorAddress[3] = 0x67; 
    CooledMattress_TemperatureSensors.TemperatureSensors[0].sensorAddress[4] = 0x99; 
    CooledMattress_TemperatureSensors.TemperatureSensors[0].sensorAddress[5] = 0xC4; 
    CooledMattress_TemperatureSensors.TemperatureSensors[0].sensorAddress[6] = 0xF7; 
    CooledMattress_TemperatureSensors.TemperatureSensors[0].sensorAddress[7] = 0x07;
    CooledMattress_TemperatureSensors.TemperatureSensors[1].deviceNameIndex_u8 = HEAT_EXCH_HOT_SIDE;
    CooledMattress_TemperatureSensors.TemperatureSensors[1].sensorAddress[0] = 0x9F;
    CooledMattress_TemperatureSensors.TemperatureSensors[1].sensorAddress[1] = 0x9D; 
    CooledMattress_TemperatureSensors.TemperatureSensors[1].sensorAddress[2] = 0x87; 
    CooledMattress_TemperatureSensors.TemperatureSensors[1].sensorAddress[3] = 0x67; 
    CooledMattress_TemperatureSensors.TemperatureSensors[1].sensorAddress[4] = 0x99; 
    CooledMattress_TemperatureSensors.TemperatureSensors[1].sensorAddress[5] = 0xC4; 
    CooledMattress_TemperatureSensors.TemperatureSensors[1].sensorAddress[6] = 0xF7;
    CooledMattress_TemperatureSensors.TemperatureSensors[1].sensorAddress[7] = 0x08;
    CooledMattress_TemperatureSensors.TemperatureSensors[2].deviceNameIndex_u8 = HEAT_EXCH_WATER_SIDE;
    CooledMattress_TemperatureSensors.TemperatureSensors[2].sensorAddress[0] = 0x9F;
    CooledMattress_TemperatureSensors.TemperatureSensors[2].sensorAddress[1] = 0x9D; 
    CooledMattress_TemperatureSensors.TemperatureSensors[2].sensorAddress[2] = 0x87; 
    CooledMattress_TemperatureSensors.TemperatureSensors[2].sensorAddress[3] = 0x67; 
    CooledMattress_TemperatureSensors.TemperatureSensors[2].sensorAddress[4] = 0x99; 
    CooledMattress_TemperatureSensors.TemperatureSensors[2].sensorAddress[5] = 0xC4; 
    CooledMattress_TemperatureSensors.TemperatureSensors[2].sensorAddress[6] = 0xF7;
    CooledMattress_TemperatureSensors.TemperatureSensors[2].sensorAddress[7] = 0x09;
    CooledMattress_TemperatureSensors.TemperatureSensors[3].deviceNameIndex_u8 = MATTRESS_OUT_WATER_TEMP;
    CooledMattress_TemperatureSensors.TemperatureSensors[3].sensorAddress[0] = 0x9F;
    CooledMattress_TemperatureSensors.TemperatureSensors[3].sensorAddress[1] = 0x9D; 
    CooledMattress_TemperatureSensors.TemperatureSensors[3].sensorAddress[2] = 0x87; 
    CooledMattress_TemperatureSensors.TemperatureSensors[3].sensorAddress[3] = 0x67; 
    CooledMattress_TemperatureSensors.TemperatureSensors[3].sensorAddress[4] = 0x99; 
    CooledMattress_TemperatureSensors.TemperatureSensors[3].sensorAddress[5] = 0xC4; 
    CooledMattress_TemperatureSensors.TemperatureSensors[3].sensorAddress[6] = 0xF7;
    CooledMattress_TemperatureSensors.TemperatureSensors[3].sensorAddress[7] = 0x0A;
    char message[DEBUG_BUFFER_SIZE];
    snprintf(message, DEBUG_BUFFER_SIZE, "Temperature sensors initialized!\n");
    logDebugMessage(message, TASKID_TEMPSENSORS);
}

void temperatureSensorsTask(void* functionInputParam){
  ControllerParameters *functionLocalControllerParameters = (ControllerParameters*)functionInputParam;
    
  while(1){
    sensors.requestTemperatures(); // Send the command to get temperatures
    
    DeviceAddress thisSensorAddress;
    /*TANK WATER TEMP ************************************************************/
    returnDeviceAddressFromName(&thisSensorAddress, TANK_WATER_TEMP);
    if(thisSensorAddress!=NULL){
        float temperatureValue = sensors.getTempC(thisSensorAddress);
        writeProtectedVariable(functionLocalControllerParameters->sensors.tankWaterTemp_f, temperatureValue, &functionLocalControllerParameters->mutexes.tankWaterTemp_mutex);
    }
    /****************************************************************************/
    
    /*HEAT EXCHANGER HOT SIDE TEMP **********************************************/
    returnDeviceAddressFromName(&thisSensorAddress, HEAT_EXCH_HOT_SIDE);
    if(thisSensorAddress!=NULL){
        float temperatureValue = sensors.getTempC(thisSensorAddress);
        writeProtectedVariable(functionLocalControllerParameters->sensors.heatExchangerHotSideTemp_f, temperatureValue, &functionLocalControllerParameters->mutexes.heatExchangerHotSideTemp_mutex);
    }
    /****************************************************************************/

    /*HEAT EXCHANGER WATER TEMP *************************************************/
    returnDeviceAddressFromName(&thisSensorAddress, HEAT_EXCH_WATER_SIDE);
    if(thisSensorAddress!=NULL){
        float temperatureValue = sensors.getTempC(thisSensorAddress);
        writeProtectedVariable(functionLocalControllerParameters->sensors.heatExchangerWaterTemp_f, temperatureValue, &functionLocalControllerParameters->mutexes.heatExchangerWaterTemp_mutex);
    }
    /****************************************************************************/

    /*MATTRESS OUTPUT WATER TEMP ************************************************/
    returnDeviceAddressFromName(&thisSensorAddress, MATTRESS_OUT_WATER_TEMP);
    if(thisSensorAddress!=NULL){
        float temperatureValue = sensors.getTempC(thisSensorAddress);
        writeProtectedVariable(functionLocalControllerParameters->sensors.mattressWaterOutputTemp_f, temperatureValue, &functionLocalControllerParameters->mutexes.mattressWaterOutputTemp_mutex);
    }
    /****************************************************************************/
    

    vTaskDelay(1000/portTICK_RATE_MS);
  }
}

void returnDeviceAddressFromName(DeviceAddress* deviceAddress_p, uint8_t DeviceNameIndex){
    for(uint8_t index = 0; index<4; index++){
        if(CooledMattress_TemperatureSensors.TemperatureSensors[index].deviceNameIndex_u8 == DeviceNameIndex){
            memcpy(*deviceAddress_p, CooledMattress_TemperatureSensors.TemperatureSensors[index].sensorAddress, 8);
            return;
        }
    }
    deviceAddress_p = NULL;
}