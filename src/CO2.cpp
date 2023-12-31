#include "CO2.h"
MQ135 GasSensor::gas_sensor = MQ135(MQ_PIN);

void GasSensor::intialize_sensor()
{
    // Setze den Pin auf input
    pinMode(MQ_PIN, INPUT);
    // float rzero = gas_sensor.getRZero();
    //  Serial.println(rzero);
}

// Lese den Sensor aus. Liefert ppm
void GasSensor::get_ppm()
{
    float ppm = gas_sensor.getPPM();
    float percent = ppm / 10000;
    Serial.println("ppm: " + (String)ppm);
    Serial.println("percent: " + (String)percent);

    // Gib das Ergebnis auf dem Bildschirm aus
    MyDisplay::display_CO2(percent, ppm);
    Util::wait_interruptable(500);
}
