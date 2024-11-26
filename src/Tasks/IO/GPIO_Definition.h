#ifndef GPIODEFINITION_H
#define GPIODEFINITION_H

#include "Arduino.h"

#define COOLEDMATTRESSIO_PIN33 GPIO_NUM_33 //MOSFETCTRL_PIN_MATTRESSPUMP
#define COOLEDMATTRESSIO_PIN32 GPIO_NUM_32 //HBRIDGE_PIN_PELTIERCELL4REV
#define COOLEDMATTRESSIO_PIN27 GPIO_NUM_27 //HBRIDGE_PIN_PELTIERCELL4FWD
#define COOLEDMATTRESSIO_PIN26 GPIO_NUM_26 //HBRIDGE_PIN_PELTIERCELL3REV
#define COOLEDMATTRESSIO_PIN25 GPIO_NUM_25 //TEMPSENSORS_CONNECTIONPIN
#define COOLEDMATTRESSIO_PIN23 GPIO_NUM_23 //PRESSURE_SENSOR_1_SCKPIN
#define COOLEDMATTRESSIO_PIN22 GPIO_NUM_22 //PRESSURE_SENSOR_1_SDIPIN
#define COOLEDMATTRESSIO_PIN21 GPIO_NUM_21 //PRESSURE_SENSOR_2_SCKPIN
#define COOLEDMATTRESSIO_PIN19 GPIO_NUM_19 //PRESSURE_SENSOR_2_SDIPIN
#define COOLEDMATTRESSIO_PIN18 GPIO_NUM_18 //PRESSURE_SENSOR_3_SCKPIN
#define COOLEDMATTRESSIO_PIN17 GPIO_NUM_17 //PRESSURE_SENSOR_4_SCKPIN
#define COOLEDMATTRESSIO_PIN16 GPIO_NUM_16 //PRESSURE_SENSOR_4_SDIPIN
#define COOLEDMATTRESSIO_PIN15 GPIO_NUM_15 //HBRIDGE_PIN_PELTIERCELL3FWD
#define COOLEDMATTRESSIO_PIN14 GPIO_NUM_14 //HBRIDGE_PIN_PELTIERCELL2REV
#define COOLEDMATTRESSIO_PIN13 GPIO_NUM_13 //HBRIDGE_PIN_PELTIERCELL2FWD
#define COOLEDMATTRESSIO_PIN5  GPIO_NUM_5  //PRESSURE_SENSOR_3_SDIPIN
#define COOLEDMATTRESSIO_PIN4  GPIO_NUM_4  //HBRIDGE_PIN_PELTIERCELL1FWD

//Strapping pin, boot fails if pulled HIGH
#define COOLEDMATTRESSIO_PIN12 GPIO_NUM_12 //HBRIDGE_PIN_PELTIERCELL1REV

//Serial RX
#define COOLEDMATTRESSIO_PIN3  GPIO_NUM_3  //UNUSED

//Already connected to onboard led
#define COOLEDMATTRESSIO_PIN2  GPIO_NUM_2  //MOSFETCTRL_PIN_RECIRCULATEPUMP

//Serial TX
#define COOLEDMATTRESSIO_PIN1  GPIO_NUM_1  //UNUSED

//High During boot, must be pulled low to enter flash mode
#define COOLEDMATTRESSIO_PIN0  GPIO_NUM_0  //MOSFETCTRL_PIN_PELTIERFANS



#endif