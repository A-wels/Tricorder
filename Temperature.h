#ifndef TEMPERATURE_CLASS
#define TEMPERATURE_CLASS

#include <DHT_U.h>
#include <DHT.h>

// DHT22
#define DHT_PIN 4
#define DHTTYPE DHT22 // DHT 11

struct dht_results
{
    float temperature;
    float humidity;
};
class MyTemperature
{

public:
    MyTemperature(uint8_t dht_pin, uint8_t dht_type);

    // initialisieren des Sensors
    void initialize();
    // Temperatur messen. Wiederholt den Vorgang bis gültiges Messergebnis vorliegt.
    dht_results measure_temperature();

private:
    DHT_Unified dht;
};

#endif