#include "Temperature.h"

MyTemperature::MyTemperature(uint8_t dht_pin = DHT_PIN, uint8_t dht_type = DHTTYPE) : dht(dht_pin, dht_type)
{
    Serial.println("DHT22 initialized");
}
void MyTemperature::initialize()
{
    dht.begin();
}
dht_results MyTemperature::measure_temperature()
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