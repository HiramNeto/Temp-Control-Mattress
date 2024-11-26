#ifndef FAN_CTRL
#define FAN_CTRL

#include <Arduino.h>
#include "../../../SharedMemory/sharedMemory.h"
#include "../../../Constants/constants.h"

void fanCtrlTask(void*);

#endif