
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "Adafruit_TCS34725.h"


#define LED_PIN 16
#define DISPLAY_SDA 5
#define DISPLAY_SCL 4
#define RGB_SDA 13
#define RGB_SCL 15

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

struct ColorData {
  uint16_t r;
  uint16_t g;
  uint16_t b;
  uint16_t c;
  uint16_t colorTemp;
  uint16_t lux;
};
ColorData before;
ColorData after;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);

  Wire.begin(DISPLAY_SDA, DISPLAY_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  Serial.println("Found display");
  delay(2000);

  Wire1.begin(RGB_SDA, RGB_SCL);
  if (tcs.begin(TCS34725_ADDRESS, &Wire1)) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  delay(200);
}


void read_colors(ColorData& cd) {
  tcs.getRawData(&cd.r, &cd.g, &cd.b, &cd.c);
  cd.colorTemp = tcs.calculateColorTemperature_dn40(cd.r, cd.g, cd.b, cd.c);
  cd.lux = tcs.calculateLux(cd.r, cd.g, cd.b);
}

void print_data(const ColorData& cd, int y) {
  display.cp437(true);
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, y);     // Start at top-left corner
  display.print("Color Temp: "); display.print(cd.colorTemp, DEC); display.print(" K - ");
  display.print("Lux: "); display.print(cd.lux, DEC); display.print(" - ");
  display.print("R: "); display.print(cd.r, DEC); display.print(" ");
  display.print("G: "); display.print(cd.g, DEC); display.print(" ");
  display.print("B: "); display.print(cd.b, DEC); display.print(" ");
  display.print("C: "); display.print(cd.c, DEC); display.print(" ");
}

#define INTEG_MS 614
#define SAMPLE_PD_MS 10*60*1000

void loop() {
  delay(INTEG_MS); // Integration time
  read_colors(before);
  digitalWrite(LED_PIN, HIGH);
  delay(INTEG_MS); // Let the value settle
  read_colors(after);
  digitalWrite(LED_PIN, LOW);
  display.clearDisplay();
  print_data(before, 0);
  print_data(after, SCREEN_HEIGHT/2 );
  display.display();
  delay(SAMPLE_PD_MS);
}
