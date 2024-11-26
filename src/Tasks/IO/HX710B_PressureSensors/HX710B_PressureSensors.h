#ifndef HX710B_PRESSURESENSORS_H
#define HX710B_PRESSURESENSORS_H

#include "Arduino.h"
#include "HX710B.h"
#include "../../../SharedMemory/sharedMemory.h"
#include "../../../Constants/constants.h"
#include "../GPIO_Definition.h"
#include "../../../Tasks/Debug/debug.h"

#define MATTRESS_INPUT_PRESSURE_ID      0
#define MATTRESS_OUTPUT_PRESSURE_ID     1
#define HEAT_EXCHANG_INPUT_PRESSURE_ID  2
#define HEAT_EXCHANG_OUTPUT_PRESSURE_ID 3

#define PRESSURE_SENSOR_1_SCKPIN COOLEDMATTRESSIO_PIN23
#define PRESSURE_SENSOR_1_SDIPIN COOLEDMATTRESSIO_PIN22
#define PRESSURE_SENSOR_2_SCKPIN COOLEDMATTRESSIO_PIN21
#define PRESSURE_SENSOR_2_SDIPIN COOLEDMATTRESSIO_PIN19
#define PRESSURE_SENSOR_3_SCKPIN COOLEDMATTRESSIO_PIN18
#define PRESSURE_SENSOR_3_SDIPIN COOLEDMATTRESSIO_PIN5
#define PRESSURE_SENSOR_4_SCKPIN COOLEDMATTRESSIO_PIN17
#define PRESSURE_SENSOR_4_SDIPIN COOLEDMATTRESSIO_PIN16

struct HX710B_PressureSensor{
    uint8_t pressureSensor_id;
    HX710B pressureSensor;
    // Construtor para inicializar diretamente com os pinos.
    HX710B_PressureSensor(uint8_t id, gpio_num_t sckPin, gpio_num_t sdiPin)
    : pressureSensor_id(id), pressureSensor(sckPin, sdiPin) {}    
};

struct Pressure_Sensors{
    HX710B_PressureSensor PressureSensorsArray[4];
};

uint8_t initPressureSensors();

void pressureSensorsTask(void* functionInputParam);

#endif