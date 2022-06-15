#ifndef TEMPERATURE_CLASS
#define TEMPERATURE_CLASS

#include <DHT_U.h>
#include <DHT.h>
#include "Definitions.h"

struct dht_results
{
    float temperature;
    float humidity;
};
class MyTemperature
{

public:
    MyTemperature(uint8_t dht_pin, uint8_t dht_type);
    // Temperatur messen. Wiederholt den Vorgang bis gültiges Messergebnis vorliegt.
    dht_results measure_temperature();

private:
    DHT_Unified dht;
};

#endif