#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <DHT_U.h>

// Ultraschall
#define TRIG_PIN 7
#define ECHO_PIN 8

// DHT11
#define DHT_PIN 2
#define DHTTYPE DHT11 // DHT 11
DHT_Unified dht(DHT_PIN, DHTTYPE);
struct dht_results
{
  float temperature;
  float humidity;
};

// Display
#define OLED_RESET 4
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

  // Temperatursensor starten
  dht.begin();
}

dht_results measure_temperature()
{
  float temperature;
  float humidity;

  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println(F("Fehler beim Messen der Temperatur"));
  }
  else
  {
    // Serial.print(F("Temperature: "));
    // Serial.print(event.temperature);
    // Serial.println(F("°C"));
    temperature = event.temperature;
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println(F("Fehler beim lesen der Luftfeuchtigkeit"));
  }
  else
  {
    // Serial.print(F("Luftfeuchtigkeit: "));
    // Serial.print(event.relative_humidity);
    // Serial.println(F("%"));
    humidity = event.relative_humidity;
  }
  return {temperature, humidity};
}

void display_temperature(dht_results dht_measurement)
{
  display.clearDisplay();
  display.cp437(true); // Erlaubt ° Zeichen
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Temperatur\n");
  display.print(dht_measurement.temperature);

  // Setze Textgröße auf 1 und schreibe °
  display.setTextSize(1);
  display.write(167);
  display.setTextSize(2);

  display.print("C");
  display.display();
}
void display_humidity(dht_results dht_measurement)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Luftf.\n");
  display.print(dht_measurement.humidity);
  display.print("%");
  display.display();
}
float measure_distance()
{
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float duration_us = pulseIn(ECHO_PIN, HIGH);
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
  display.print(distanz);
  display.print("cm");
  display.display();
}

void loop()
{
  Serial.println("Measuring...");
  dht_results dht_measurement = measure_temperature();
  display_temperature(dht_measurement);
  Serial.println(dht_measurement.temperature);
  delay(2000);
  display_humidity(dht_measurement);
  delay(2000);
  displayDistance();
  delay(2000);
}