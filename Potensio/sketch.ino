#define POT_PIN 1

void TaskPot(void *pvParameters) {
  while (1) {
    int value = analogRead(POT_PIN);
    Serial.println(value);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  // pilih core
  // xTaskCreatePinnedToCore(TaskPot, "POT", 2048, NULL, 1, NULL, 0); // core 0
  xTaskCreatePinnedToCore(TaskPot, "POT", 2048, NULL, 1, NULL, 1); // core 1
}

void loop() {}
