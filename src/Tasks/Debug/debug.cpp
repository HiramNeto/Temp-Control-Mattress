#include "debug.h"

// Declaração da fila
extern QueueHandle_t debugQueue;

// Inicializa a fila de debug
void initializeDebugBuffer();

QueueHandle_t debugQueue = NULL;

void initializeDebugBuffer() {
    // Cria a fila de debug
    debugQueue = xQueueCreate(DEBUG_QUEUE_LENGTH, DEBUG_BUFFER_SIZE * sizeof(char));
    if (debugQueue == NULL) {
      Serial.println("DebugError: Falha ao criar a fila de debug");
    }else{
      Serial.println("Fila de impressão criada com sucesso!");
    }
}

bool logDebugMessage(const char* message, uint8_t taskId) {
    if (debugQueue == NULL) {
        Serial.println("DebugError: Fila de debug não inicializada!");
        return false;
    }

    // Buffer para a mensagem final (identificação + mensagem)
    char formattedMessage[DEBUG_BUFFER_SIZE];
    
    // Formata a mensagem com o ID da task
    switch(taskId){
      case TASKID_TEMPCTRL:{
        snprintf(formattedMessage, DEBUG_BUFFER_SIZE, "Temperature control task: %s", message);
      }break;
      case TASKID_FANCTRL:{
        snprintf(formattedMessage, DEBUG_BUFFER_SIZE, "Fan control task: %s", message);
      }break;
      case TASKID_TEMPSENSORS:{
        snprintf(formattedMessage, DEBUG_BUFFER_SIZE, "Temperature sensors task: %s", message);
      }break;
      case TASKID_PWMMOSFETS:{
        snprintf(formattedMessage, DEBUG_BUFFER_SIZE, "PWM mosfets task: %s", message);
      }break;
      case TASKID_PRESSURESENSORS:{
        snprintf(formattedMessage, DEBUG_BUFFER_SIZE, "Pressure sensors task: %s", message);
      }break;
      default:{
        snprintf(formattedMessage, DEBUG_BUFFER_SIZE, "Task %u: %s", taskId, message);
      }break;
    }
    
    // Envia a mensagem para a fila
    if (xQueueSend(debugQueue, formattedMessage, 0) == pdTRUE) {
        return true;
    } else {
        Serial.println("DebugError: Fila de debug cheia!");
        return false;
    }
}

void print_task(void* functionInputParam){
  ControllerParameters *functionLocalControllerParameters = (ControllerParameters*)functionInputParam;
  char buffer[DEBUG_BUFFER_SIZE];

  initializeDebugBuffer();   
  
  while(1){
    if (xQueueReceive(debugQueue, buffer, portMAX_DELAY) == pdTRUE) {
      // Imprime a mensagem no console
      Serial.println(buffer);
    }
    vTaskDelay(1/portTICK_RATE_MS);
  }
}