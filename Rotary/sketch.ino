#define CLK 18
#define DT  19

volatile int count = 0;
int lastState;

void TaskEncoder(void *pvParameters) {
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);

  lastState = digitalRead(CLK);

  while (1) {
    int currentState = digitalRead(CLK);

    if (currentState != lastState) {
      if (digitalRead(DT) != currentState) count++;
      else count--;

      Serial.print("Encoder: ");
      Serial.println(count);
    }

    lastState = currentState;
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  // pilih core
  // xTaskCreatePinnedToCore(TaskEncoder, "ENC", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskEncoder, "ENC", 2048, NULL, 1, NULL, 1);
}

void loop() {}
