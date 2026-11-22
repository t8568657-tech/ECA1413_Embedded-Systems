#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t binarySemaphore;

void BinaryProducerTask(void *pvParameters) {
  int eventNumber = 0;
  while (1) {
    eventNumber++;
    Serial.printf("[BINARY] Producer generated event %d\n", eventNumber);
    if (xSemaphoreGive(binarySemaphore) == pdTRUE)
      Serial.println("[BINARY] Event signal sent");
    else
      Serial.println("[BINARY] Signal not added; semaphore already full");
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void BinaryConsumerTask(void *pvParameters) {
  while (1) {
    if (xSemaphoreTake(binarySemaphore, portMAX_DELAY) == pdTRUE)
      Serial.println("[BINARY] Consumer received signal");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("BINARY SEMAPHORE EVENT SIGNALING");
  binarySemaphore = xSemaphoreCreateBinary();
  if (binarySemaphore == NULL) {
    Serial.println("ERROR: Binary semaphore creation failed");
    while (1) vTaskDelay(pdMS_TO_TICKS(1000));
  }
  xTaskCreate(BinaryProducerTask, "BinProd", 4096, NULL, 2, NULL);
  xTaskCreate(BinaryConsumerTask, "BinCons", 4096, NULL, 1, NULL);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}
