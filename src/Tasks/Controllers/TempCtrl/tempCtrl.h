#ifndef TEMPCTRL_H
#define TEMPCTRL_H

#include <Arduino.h>
#include <Fuzzy.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../../../SharedMemory/sharedMemory.h"
#include "../../../Constants/constants.h"
#include "../../../Tasks/Debug/debug.h"

void tempCtrlTask(void*);

#endif