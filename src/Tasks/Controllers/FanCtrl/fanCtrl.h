#ifndef FAN_CTRL
#define FAN_CTRL

#include <Arduino.h>
#include "../../../SharedMemory/sharedMemory.h"
#include "../../../Constants/constants.h"
#include "../../../Tasks/Debug/debug.h"

void fanCtrlTask(void*);

#endif