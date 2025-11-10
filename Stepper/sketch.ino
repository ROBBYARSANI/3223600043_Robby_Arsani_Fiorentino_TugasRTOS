#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, 13, 12);

void TaskStepper(void *pvParameters) {
  stepper.setMaxSpeed(800);
  stepper.setAcceleration(400);

  while (1) {
    Serial.println("Maju"); 
    stepper.moveTo(stepper.currentPosition() + 200);

    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }
    vTaskDelay(pdMS_TO_TICKS(200));

    Serial.println("Mundur"); 
    stepper.moveTo(stepper.currentPosition() - 200);
    
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

void setup() {
  Serial.begin(115200); 

  // pilih core 0 / 1
  // xTaskCreatePinnedToCore(TaskStepper, "STEP", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskStepper, "STEP", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // Kosong, semua dikerjakan oleh TaskStepper
}