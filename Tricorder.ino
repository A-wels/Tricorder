#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
#define TRIG_PIN 7
#define ECHO_PIN 8
Adafruit_SSD1306 display(OLED_RESET);

void setup()
{
  // Pins für Ultraschall
  pinMode(TRIG_PIN, OUTPUT); // Setzt den vorher definierten trigPin als Output.
  pinMode(ECHO_PIN, INPUT);  // Setzt den vorher defnierten echoPin als Input.

  // Bildschirm
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Serial Monitor
  Serial.begin(9600);
  Serial.println("Starting...");
}

float measure_distance()
{
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float duration_us = pulseIn(8, HIGH);
  Serial.println(duration_us);
  return 0.017 * duration_us;
}

void displayDistance()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Distanz:\n");
  float distanz = 0;
  while (distanz == 0)
  {
    distanz = measure_distance();
  }
  Serial.println(distanz);
  display.print(distanz);
  display.print("cm");
  display.display();
}

void loop()
{
  displayDistance();
  delay(1000);
}