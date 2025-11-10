#define LED1 1
#define LED2 2
#define LED3 42

void TaskLED(void *pvParameters) {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  while (1) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // Ganti core ke 0 atau 1 untuk pengujian:
  //xTaskCreatePinnedToCore(TaskLED, "LED3", 2048, NULL, 1, NULL, 0);  // core 0
  xTaskCreatePinnedToCore(TaskLED, "LED3", 2048, NULL, 1, NULL, 1);  // core 1
}

void loop() {}
