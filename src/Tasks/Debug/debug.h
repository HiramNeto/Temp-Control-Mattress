#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "../../SharedMemory/sharedMemory.h"
#include <freertos/queue.h>

// Configurações do buffer de debug
#define DEBUG_BUFFER_SIZE 128  // Tamanho do buffer para cada mensagem
#define DEBUG_QUEUE_LENGTH 10  // Quantidade máxima de mensagens no buffer

// Funções para log de debug
bool logDebugMessage(const char* message,uint8_t taskId);

void print_task(void*);

#endif