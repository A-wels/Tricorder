#include "CO2.h"
MQ135 GasSensor::gas_sensor = MQ135(MQ_PIN);

void GasSensor::intialize_sensor()
{
    float rzero = gas_sensor.getRZero();
    delay(3000);
    Serial.print("MQ135 RZERO Calibration Value : ");
    Serial.println(rzero);
}

void GasSensor::get_ppm()
{
    float ppm = gas_sensor.getPPM();
    float percent = ppm / 1000;
    Serial.println(ppm);
    Serial.println("percent: " + (String)percent);
    MyDisplay::display_CO2(percent, ppm);
    Util::wait_interruptable(500);
}
