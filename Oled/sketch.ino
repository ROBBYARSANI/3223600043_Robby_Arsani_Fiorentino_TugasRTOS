#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int counter = 0;

void TaskOLED(void *pvParameters) {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed!");
    vTaskDelete(NULL);
  }

  while (1) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("Test OLED");
    display.println("Running Task");

    display.print("Counter: ");
    display.println(counter);

    display.print("Core: ");
    display.println(xPortGetCoreID());

    display.display();

    counter++;
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  // pilih core 0 atau core 1
  // xTaskCreatePinnedToCore(TaskOLED, "OLED", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskOLED, "OLED", 4096, NULL, 1, NULL, 1);
}

void loop() {}
