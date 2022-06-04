#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <DHT_U.h>

// Libraries for PN532
// https://github.com/elechouse/PN532
// Auf Basis von: https://warlord0blog.wordpress.com/2021/10/09/esp32-and-nfc-over-i2c/
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>
PN532_I2C pn532i2c(Wire);
PN532 nfc(pn532i2c);
volatile bool nfc_connected = false;

// Anzahld der Module
#define NUMBER_OF_MODULES 3

// Ultraschall
#define TRIG_PIN 14
#define ECHO_PIN 13

// DHT22
#define DHT_PIN 4
#define DHTTYPE DHT22 // DHT 11
DHT_Unified dht(DHT_PIN, DHTTYPE);
struct dht_results
{
  float temperature;
  float humidity;
};

// Display
#define OLED_RESET -1

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Potentiometer
#define POTENTIO_PIN 34
int pot_val;

// Temperatur messen. Wiederholt den Vorgang bis gültiges Messergebnis vorliegt.
dht_results measure_temperature()
{
  float temperature = 0;
  float humidity = 0;
  sensors_event_t event;
  while (temperature == 0.0 || humidity == 0.0)
  {
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
      Serial.println(F("Fehler beim Messen der Temperatur"));
    }
    else
    {
      temperature = event.temperature;
    }

    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity))
    {
      Serial.println(F("Fehler beim lesen der Luftfeuchtigkeit"));
    }
    else
    {
      humidity = event.relative_humidity;
    }
  }
  return {temperature, humidity};
}

// Zeige Temperatur auf dem Display an
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

// Zeige Luftfeuchtigkeit auf dem Bildschirm an
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

// Miss die Distanz.
float measure_distance()
{
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float duration_us = pulseIn(ECHO_PIN, HIGH);
  return 0.017 * duration_us;
}

// Misst die Distanz und zeigt die Distanz auf dem Bildschirm an. Messung wird wiederholt bis eine gültige vorliegt.
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
int dots = 0;
int count_direction = 1;

// Bildschirmausgabe für NFC
void display_NFC()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Scanne");
  for (int i = 0; i < dots; i++)
  {
    display.print(".");
  }
  if (dots == 0)
  {
    count_direction = 1;
  }
  else if (dots == 3)
  {
    count_direction = -1;
  }
  dots = dots + count_direction;

  display.println("");
  display.display();
}

// Beliebigen text ausgeben
void display_text(String text)
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print(text);
  display.display();
}
// Task: Lese das Potentiometer dauerthaft aus. Wird in 2. Thread ausgeführt.
void read_potentiometer(void *parameter)
{

  while (true)
  {
    pot_val = analogRead(POTENTIO_PIN);
    pot_val = map(pot_val, 0, 4095, 0, NUMBER_OF_MODULES - 1); // Map auf die Anzahl der Module
    delay(100);
  }
}

bool connect_nfc()
{
  nfc.begin();
  // Connected, show version
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata)
  {
    display_text("Scanner\n defekt.");
    Serial.println("PN53x card not found!");
    return false;
  }

  // port
  Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware version: ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for card (ISO14443A Mifare)...");
  Serial.println("");

  return true;
}

// Lese einen NFC Chip mit dem PN532 (NFC Reader) aus
void read_nfc()
{
  // Wenn pot_val abweicht, so wurde die aktuelle Aufgabe geändert

  int task_at_start = pot_val;
  boolean success;

  // Buffer: Hier wird die UID des NFC Chips gespeichert
  uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};

  // UID Länge
  uint8_t uid_length;

  // Stelle eine Verbindung
  nfc_connected = false;
  display_text("Starte\nScanner...");
  delay(150);
  // while (!nfc_connected && task_at_start == pot_val)
  while (task_at_start == pot_val)
  {
    Serial.println(task_at_start);
    Serial.println(pot_val);
    Serial.println("");
    boolean success;
    // Buffer to store the UID
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};

    // UID size (4 or 7 bytes depending on card type)
    uint8_t uidLength;

    // Stelle eine Verbindung mit dem NFC Modul her
    while (!nfc_connected && task_at_start == pot_val)
    {
      nfc_connected = connect_nfc();
    }
    if (pot_val != task_at_start)
    {
      break;
    }

    // Versuche einen NFC Chip auszulesen
    display_NFC();

    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

    // Falls erkannt: Datenausgabe
    if (success)
    {
      display_text("Scan\nerfolgreich");

      Serial.println("Card Detected");
      Serial.print("Size of UID: ");
      Serial.print(uidLength, DEC);
      Serial.println(" bytes");
      Serial.print("UID: ");
      for (uint8_t i = 0; i < uidLength; i++)
      {
        Serial.print(" 0x");
        Serial.print(uid[i], HEX);
      }
      Serial.println("");
      Serial.println("");

      delay(1000);
    }
    else
    {
      // PN532 probably timed out waiting for a card
      Serial.println("Timed out waiting for a card");
    }
    nfc_connected = connect_nfc();
  }
}
// SETUP -------------------------------------------------

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Serial.println("Starting...");

  // Pins für Ultraschall
  pinMode(TRIG_PIN, OUTPUT); // Setzt den vorher definierten trigPin als Output.
  pinMode(ECHO_PIN, INPUT);  // Setzt den vorher defnierten echoPin als Input.

  // Pin für Potentiometer
  pinMode(POTENTIO_PIN, INPUT);

  // Task für Potentiometer polling
  TaskHandle_t xHandle = NULL;
  xTaskCreate(read_potentiometer, "Read Potentiometer", 1000, NULL, 1, &xHandle);

  // Bildschirm starten
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }
  display_text("System\nboot");

  // Temperatursensor starten
  dht.begin();
}

// Wartet ungefähr für die angegebene Dauer. Wird vorzeitig abgebrochen, wenn pot_val sich verändert.
void wait_interruptable(int duration)
{
  int time_start = millis();
  int pot_at_start = pot_val;
  while (millis() - time_start < duration && pot_val == pot_at_start)
  {
    delay(100);
  }
  return;
}

void loop()
{
  switch (pot_val)
  {
    // Case 0: Thermometer
  case 0:
  {
    dht_results dht_measurement = measure_temperature();
    display_temperature(dht_measurement);
    wait_interruptable(2000);
    if (pot_val != 0)
    {
      break;
    }
    display_humidity(dht_measurement);
    wait_interruptable(2000);
    break;
  }
    // Case 1: Ultraschall
  case 1:
  {
    displayDistance();
    wait_interruptable(2000);
    break;
  }

    // Case 2: NFC
  case 2:
  {
    read_nfc();
    break;
  }
  }
}