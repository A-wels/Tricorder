#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

// Libraries for PN532
// https://github.com/elechouse/PN532
// Auf Basis von: https://warlord0blog.wordpress.com/2021/10/09/esp32-and-nfc-over-i2c/
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

#include "src/Display.h"
#include "src/Ultraschall.h"
#include "src/Util.h"
#include "src/NFC.h"
#include "src/Potentiometer.h"
#include "src/Definitions.h"

// Objekt für DHT22
MyTemperature dht(DHT_PIN, DHTTYPE);

// Objekt für Ultraschall
MyUltraschall ultraschall;

// SETUP -------------------------------------------------

void setup()
{
  // Serial Monitor
  Serial.begin(9600);
  Serial.println("Starting...");

  // Potentiometer initialisieren
  MyPotentiometer::intialize_potentiometer();

  // Ultraschall initialisieren
  ultraschall.initialize();

  // Display starten
  MyDisplay::initialize_display();
}
void loop()
{
  switch (Util::pot_val)
  {
    // Case 0: Thermometer
  case 0:
  {
    // Miss die Temperatur und zeige sie für 2 Sekunden an
    dht_results dht_measurement = dht.measure_temperature();
    MyDisplay::display_temperature(dht_measurement);
    Util::wait_interruptable(2000);

    // Miss die Luftfeuchtigkeit und zeige sie für 2 Sekunden an
    MyDisplay::display_humidity(dht_measurement);
    Util::wait_interruptable(2000);
    break;
  }
    // Case 1: Ultraschall
  case 1:
  {
    // Miss die Distanz und zeige sie für 200ms an
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
    // Versuche einen NFC Chip zu lesen
    MyNFC::read_nfc();
    break;
  }
  }
}