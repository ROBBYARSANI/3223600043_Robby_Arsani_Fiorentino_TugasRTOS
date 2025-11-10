#define BUTTON1 4
#define BUTTON2 6

void TaskButton(void *pvParameters) {
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  while (1) {
    if (digitalRead(BUTTON1) == LOW) {
      Serial.println("Button 1 pressed!");
      vTaskDelay(200 / portTICK_PERIOD_MS);  // debounce
    }

    if (digitalRead(BUTTON2) == LOW) {
      Serial.println("Button 2 pressed!");
      vTaskDelay(200 / portTICK_PERIOD_MS);  // debounce
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  // Jalankan di Core 0 / Core 1 (pilih salah satu)
  //xTaskCreatePinnedToCore(TaskButton, "BUTTONS", 2048, NULL, 1, NULL, 0); // core 0
  xTaskCreatePinnedToCore(TaskButton, "BUTTONS", 2048, NULL, 1, NULL, 1); // core 1
}

void loop() {}
