#ifndef DS18B20_TEMPERATURESENSORS_H
#define DS18B20_TEMPERATURESENSORS_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Arduino.h>
#include "../../../SharedMemory/sharedMemory.h"
#include "../GPIO_Definition.h"
#include "../../../Tasks/Debug/debug.h"

#define TEMPSENSORS_CONNECTIONPIN COOLEDMATTRESSIO_PIN25

#define TANK_WATER_TEMP 0
#define HEAT_EXCH_HOT_SIDE 1
#define HEAT_EXCH_WATER_SIDE 2
#define MATTRESS_OUT_WATER_TEMP 3

struct DS18B20_TemperatureSensor{
    DeviceAddress sensorAddress;
    uint8_t deviceNameIndex_u8;
};

struct Temperature_Sensors{
    DS18B20_TemperatureSensor TemperatureSensors[4];
};

void initTemperatureSensors();
void temperatureSensorsTask(void*);

#endif