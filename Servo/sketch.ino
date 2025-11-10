#include <ESP32Servo.h>

#define SERVO_PIN 5
Servo myservo;

void TaskServo(void *pvParameters) {
  myservo.setPeriodHertz(50);
  myservo.attach(SERVO_PIN, 500, 2400);

  while (1) {
    myservo.write(0);   vTaskDelay(1000);
    myservo.write(90);  vTaskDelay(1000);
    myservo.write(180); vTaskDelay(1000);
  }
}

void setup() {
  // pilih core
  // xTaskCreatePinnedToCore(TaskServo, "SERVO", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskServo, "SERVO", 4096, NULL, 1, NULL, 1);
}

void loop() {}
