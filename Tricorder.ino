#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

#include "src/Display.h"
#include "src/Ultraschall.h"
#include "src/Util.h"
#include "src/NFC.h"
#include "src/Potentiometer.h"
#include "src/Definitions.h"
#include "src/Heartbeat.h"
#include "src/CO2.h"
#include "src/Compass.h"
#include "src/LED.h"

// Objekt für DHT22
MyTemperature dht(DHT_PIN, DHTTYPE);

// Objekt für Ultraschall
MyUltraschall ultraschall;

// SETUP -------------------------------------------------

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Serial.println("Starting...");
  // LEDs initialisieren
  MyLED::initialize();
  delay(500);
  // Display starten
  MyDisplay::initialize_display();

  // NFC initialisieren
  MyNFC::connect_nfc();

  // Potentiometer initialisieren
  MyPotentiometer::intialize_potentiometer();

  // Ultraschall initialisieren
  ultraschall.initialize();

  // Gassensor initialisieren
  GasSensor::intialize_sensor();

  // Herzschlag sensor
  pinMode(HEARTBEAT_PIN, INPUT);

  // Kompass starten
  Compass::intialize();
}
void loop()
{
  Serial.println(Util::pot_val);

  switch (Util::pot_val)
  {
    // Case 0: Thermometer
  case 0:
  {
    // Miss die Temperatur und zeige sie für 2 Sekunden an
    dht_results dht_measurement = dht.measure_temperature();
    MyDisplay::display_temperature(dht_measurement);
    MyLED::blink_led(MyLED::colors::YELLOW, 200);

    Util::wait_interruptable(2000);

    // Miss die Luftfeuchtigkeit und zeige sie für 2 Sekunden an
    MyDisplay::display_humidity(dht_measurement);
    MyLED::blink_led(MyLED::colors::YELLOW, 200);

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
    MyLED::blink_distance(distance);
    MyDisplay::display_distance(distance);
    break;
  }

    // Case 2: NFC
  case 2:
  {
    // Versuche einen NFC Chip zu lesen
    MyNFC::read_nfc();
    break;
  }
  // Case 3: Heartbeat
  case 3:
  {
    Heartbeat::measure_heartbeat();
    break;
  }

  // Case 4: CO2 Sensor
  case 4:
  {
    MyLED::disable_all_leds();
    GasSensor::get_ppm();
    break;
  }
    // Case 5: Compass
  case 5:
  {
    MyLED::disable_all_leds();
    MyLED::blink_led(MyLED::colors::RED, 400);
    MyLED::blink_led(MyLED::colors::YELLOW, 400);

    Compass::get_readings();
    break;
  }
  }
}
