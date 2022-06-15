#include <SPI.h>
#include <Wire.h>

#include <Adafruit_Sensor.h>
#include "Display.h"
#include "Ultraschall.h"
#include "Util.h"
#include "NFC.h"

// Libraries for PN532
// https://github.com/elechouse/PN532
// Auf Basis von: https://warlord0blog.wordpress.com/2021/10/09/esp32-and-nfc-over-i2c/
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

// Objekt für DHT22
MyTemperature dht(DHT_PIN, DHTTYPE);

// Objekt für Ultraschall
MyUltraschall ultraschall;

// Anzahld der Module
#define NUMBER_OF_MODULES 3

// Potentiometer
#define POTENTIO_PIN 34

// Task: Lese das Potentiometer dauerthaft aus. Wird in 2. Thread ausgeführt.
void read_potentiometer(void *parameter)
{

  while (true)
  {
    Util::pot_val = analogRead(POTENTIO_PIN);
    Util::pot_val = map(Util::pot_val, 0, 4095, 0, NUMBER_OF_MODULES - 1); // Map auf die Anzahl der Module
    delay(100);
  }
}

// SETUP -------------------------------------------------

void setup()
{
  // Serial Monitor
  Serial.begin(9600);
  Serial.println("Starting...");

  // Ultraschall initialisieren
  ultraschall.initialize();

  // Pin für Potentiometer
  pinMode(POTENTIO_PIN, INPUT);

  // Task für Potentiometer polling
  TaskHandle_t xHandle = NULL;
  xTaskCreate(read_potentiometer, "Read Potentiometer", 1000, NULL, 1, &xHandle);

  // Display starten
  MyDisplay::initialize_display();

  // Temperatursensor starten
  dht.initialize();
}

void loop()
{
  switch (Util::pot_val)
  {
    // Case 0: Thermometer
  case 0:
  {
    dht_results dht_measurement = dht.measure_temperature();
    MyDisplay::display_temperature(dht_measurement);
    Util::wait_interruptable(2000);
    if (Util::pot_val != 0)
    {
      break;
    }
    MyDisplay::display_humidity(dht_measurement);
    Util::wait_interruptable(2000);
    break;
  }
    // Case 1: Ultraschall
  case 1:
  {
    float distance = 0;
    while (distance == 0)
    {
      distance = ultraschall.measure_distance();
    }
    MyDisplay::display_distance(distance);
    Util::wait_interruptable(200);
    break;
  }

    // Case 2: NFC
  case 2:
  {
    MyNFC::read_nfc();
    break;
  }
  }
}