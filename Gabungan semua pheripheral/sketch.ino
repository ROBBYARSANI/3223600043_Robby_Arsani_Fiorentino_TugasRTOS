
// ===================== Libraries =====================
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#include <AccelStepper.h>

// ===================== OLED SETUP =====================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===================== Servo ==========================
Servo myservo;

// ===================== Stepper ========================
AccelStepper stepper(AccelStepper::DRIVER, 13, 12);

// ===================== Encoder ========================
volatile int encoderCount = 0;
int lastStateCLK;

// ===================== Pin Assignments =================
#define LED_PIN 2
#define BUZZ_PIN 4
#define BUTTON_PIN 15
#define POT_PIN 1
#define SERVO_PIN 17
#define CLK_PIN 18
#define DT_PIN 19

// =======================================================
// ====================== TASKS ==========================
// =======================================================

// ---------- LED BLINK TASK (CORE 0) ----------
void TaskLED(void *pvParameters) {
  pinMode(LED_PIN, OUTPUT);
  while (1) {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// ---------- BUZZER TASK (CORE 1) ----------
void TaskBuzzer(void *pvParameters) {
  pinMode(BUZZ_PIN, OUTPUT);
  Serial.println("[BUZZER] Task Started - Manual PWM");
  
  while (1) {
    // Manual PWM untuk buzzer (hindari tone() yang bentrok dengan servo)
    for (int i = 0; i < 300; i++) {
      digitalWrite(BUZZ_PIN, HIGH);
      delayMicroseconds(625);  // ~800Hz
      digitalWrite(BUZZ_PIN, LOW);
      delayMicroseconds(625);
    }
    vTaskDelay(700 / portTICK_PERIOD_MS);
  }
}

// ---------- BUTTON TASK (CORE 0) ----------
void TaskButton(void *pvParameters) {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  while (1) {
    if (digitalRead(BUTTON_PIN) == LOW) {
      Serial.println("[BUTTON] Pressed!");
      vTaskDelay(200 / portTICK_PERIOD_MS);
    }
  }
}

// ---------- POTENTIOMETER TASK (CORE 1) ----------
void TaskPot(void *pvParameters) {
  while (1) {
    int value = analogRead(POT_PIN);
    Serial.print("[POT] Value: ");
    Serial.println(value);
    vTaskDelay(150 / portTICK_PERIOD_MS);
  }
}

// ---------- OLED TASK (CORE 0) ----------
void TaskOLED(void *pvParameters) {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Failed!");
    vTaskDelete(NULL);
  }
  display.clearDisplay();

  while (1) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("ESP32-S3");
    display.println("Multitasking");
    display.println("All Peripheral OK!");
    display.display();
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

// ---------- ENCODER TASK (CORE 1) ----------
void TaskEncoder(void *pvParameters) {
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);

  lastStateCLK = digitalRead(CLK_PIN);

  while (1) {
    int currentStateCLK = digitalRead(CLK_PIN);

    if (currentStateCLK != lastStateCLK) {
      if (digitalRead(DT_PIN) != currentStateCLK) encoderCount++;
      else encoderCount--;

      Serial.print("[ENCODER] Count: ");
      Serial.println(encoderCount);
    }

    lastStateCLK = currentStateCLK;
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}

// ---------- SERVO TASK (CORE 0) ----------
void TaskServo(void *pvParameters) {
  myservo.setPeriodHertz(50);
  myservo.attach(SERVO_PIN, 500, 2400);

  while (1) {
    myservo.write(0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    myservo.write(90);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    myservo.write(180);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// ---------- STEPPER TASK (CORE 1) ----------
void TaskStepper(void *pvParameters) {
  stepper.setMaxSpeed(800);
  stepper.setAcceleration(400);

  while (1) {
    stepper.moveTo(stepper.currentPosition() + 800);
    while (stepper.distanceToGo() != 0) stepper.run();
    vTaskDelay(300 / portTICK_PERIOD_MS);

    stepper.moveTo(stepper.currentPosition() - 800);
    while (stepper.distanceToGo() != 0) stepper.run();
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

// =======================================================
// ======================= SETUP =========================
// =======================================================
void setup() {
  Serial.begin(115200);

  // Core 0 Tasks
  xTaskCreatePinnedToCore(TaskLED, "LED", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskButton, "BUTTON", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskOLED, "OLED", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskServo, "SERVO", 4096, NULL, 1, NULL, 0);

  // Core 1 Tasks
  xTaskCreatePinnedToCore(TaskBuzzer, "BUZZER", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskPot, "POT", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskEncoder, "ENCODER", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskStepper, "STEPPER", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // Loop kosong → semua dikerjakan oleh task
}
