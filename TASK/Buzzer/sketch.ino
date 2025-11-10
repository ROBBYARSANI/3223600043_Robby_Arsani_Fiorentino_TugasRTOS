#define BUZZ_PIN 4

void TaskBuzzer(void *pvParameters) {
  pinMode(BUZZ_PIN, OUTPUT);

  while (1) {
    tone(BUZZ_PIN, 1000);
    vTaskDelay(300 / portTICK_PERIOD_MS);

    noTone(BUZZ_PIN);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // pilih core
  //xTaskCreatePinnedToCore(TaskBuzzer, "BUZZ", 2048, NULL, 1, NULL, 0); // core 0
  xTaskCreatePinnedToCore(TaskBuzzer, "BUZZ", 2048, NULL, 1, NULL, 1); // core 1
}

void loop() {}
