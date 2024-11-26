#include "test.h"

void test_function(void* functionInputParam){
  ControllerParameters *functionLocalControllerParameters = (ControllerParameters*)functionInputParam;

  randomSeed(analogRead(0));

  while(1){
    // Gera um número aleatório entre 0 e 2500 e depois divide por 100 para obter um valor de ponto flutuante
    float newTankWaterTemperature = random(0, 2501) / 100.0;

    // Tentativa de ajustar a temperatura da água do tanque
    if (writeProtectedVariable(functionLocalControllerParameters->sensors.tankWaterTemp_f, newTankWaterTemperature, &functionLocalControllerParameters->mutexes.tankWaterTemp_mutex)) {
        //Serial.printf("Nova temperatura da água do tanque ajustada para: %.2f°C\n", newTankWaterTemperature);
    } else {
        //Serial.println("Não foi possível ajustar a temperatura da água do tanque.");
    }
    vTaskDelay(1000/portTICK_RATE_MS);
  }
}