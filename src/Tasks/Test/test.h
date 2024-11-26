#ifndef TEST_H
#define TEST_H
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../../SharedMemory/sharedMemory.h"

void test_function(void* functionInputParam);


#endif