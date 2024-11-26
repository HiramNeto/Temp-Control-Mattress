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
    
    /*float tankWaterTemp;
    if (readProtectedVariable(tankWaterTemp, &functionLocalControllerParameters->sensors.tankWaterTemp_f, &functionLocalControllerParameters->mutexes.tankWaterTemp_mutex)) {
      Serial.printf("Temperatura do tanque: %.2f",tankWaterTemp);
      Serial.println();
    }else{
      Serial.println("Erro ao tentar acessar o valor da temperatura de água!");
    }
    uint32_t mattressInputPressure;
    if (readProtectedVariable(mattressInputPressure, &functionLocalControllerParameters->sensors.mattressInputPressure_u32, &functionLocalControllerParameters->mutexes.mattressInputPressure_mutex)) {
      Serial.print("Pressão de entrada do colchão: ");
      Serial.println(mattressInputPressure);
    }else{
      Serial.println("Erro ao tentar acessar o valor da pressão de entrada do colchão!");
    }

    static uint8_t actuatorStep = 0;
    switch(actuatorStep){
      case 0:
      case 4:{
        Serial.println("Fans at 0%");
        writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerFansPercent_f, (float)0.0, &functionLocalControllerParameters->mutexes.heatExchangerFansPercent_mutex);
      }break;
      case 1:
      case 3:{
        Serial.println("Fans at 50%");
        writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerFansPercent_f, (float)50.0, &functionLocalControllerParameters->mutexes.heatExchangerFansPercent_mutex);
      }break;
      case 2:{
        Serial.println("Fans at 100%");
        writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerFansPercent_f, (float)100.0, &functionLocalControllerParameters->mutexes.heatExchangerFansPercent_mutex);
      }break;
      case 5:
      case 9:{
        Serial.println("Mattress pump at 0%");
        writeProtectedVariable(functionLocalControllerParameters->actuators.mattressPumpPercent_f, (float)0.0, &functionLocalControllerParameters->mutexes.mattressPumpPercent_mutex);
      }break;
      case 6:
      case 8:{
        Serial.println("Mattress pump at 50%");
        writeProtectedVariable(functionLocalControllerParameters->actuators.mattressPumpPercent_f, (float)50.0, &functionLocalControllerParameters->mutexes.mattressPumpPercent_mutex);
      }break;
      case 7:{
        Serial.println("Mattress pump at 100%");
        writeProtectedVariable(functionLocalControllerParameters->actuators.mattressPumpPercent_f, (float)100.0, &functionLocalControllerParameters->mutexes.mattressPumpPercent_mutex);
      }break;
      case 10:
      case 12:
      case 14:{
        Serial.println("Heat Exchanger pump OFF");
        writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPump_b, (boolean)false, &functionLocalControllerParameters->mutexes.heatExchangerPump_mutex);
      }break;
      case 11:
      case 13:{
        Serial.println("Heat Exchanger pump ON");
        writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPump_b, (boolean)true, &functionLocalControllerParameters->mutexes.heatExchangerPump_mutex);
      }break;
      case 15:
      case 19:{
        Serial.println("Heat Exchanger peltier 1 OFF");
        writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_OFF, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
      }break;
      case 16:{
        Serial.println("Heat Exchanger peltier 1 COOLING");
        writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_COOLING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
      }break;
      case 17:
      case 18:{
        Serial.println("Heat Exchanger peltier 1 HEATING");
        writeProtectedVariable(functionLocalControllerParameters->actuators.heatExchangerPeltier1_u8, (uint8_t)PELTIER_HEATING_ON, &functionLocalControllerParameters->mutexes.heatExchangerPeltier1_mutex);
      }break;
    }
    actuatorStep = (actuatorStep==19)?0:actuatorStep+1;*/

    vTaskDelay(1000/portTICK_RATE_MS);
  }
}