#include "Compass.h"
Adafruit_HMC5883_Unified Compass::compass = Adafruit_HMC5883_Unified(47);

void Compass::intialize()
{
    if (!Compass::compass.begin())
    {
        String text[] = {"Kompass", "Defekt"};
        MyDisplay::display_text(text, 2);
        Serial.println("HMC5388 nicht erkannt");
        Util::wait_interruptable(10000);
    }
}

void Compass::get_readings()
{

    // Events vom Kompass werden in "event" gespeichert
    sensors_event_t event;
    compass.getEvent(&event);

    Serial.print("X: ");
    Serial.print(event.magnetic.x);
    Serial.print(" ");
    Serial.print("Y: ");
    Serial.print(event.magnetic.y);
    Serial.print(" ");
    Serial.print("Z: ");
    Serial.print(event.magnetic.z);
    Serial.print(" ");
    Serial.println("uT");

    // Richtung bestimmen mittels arctangent
    float heading = atan2(event.magnetic.y, event.magnetic.x);
    heading += MAGNETIC_DECLINATION;

    // Negative Zahlen abfangen
    if (heading < 0)
    {
        heading += 2 * M_PI;
    }

    // Überlauf durch magnetic declination ausgleich abfangen
    if (heading > 2 * M_PI)
    {
        heading -= 2 * M_PI;
    }

    float heading_degree = heading * 180 / M_PI;

    Serial.println("Heading: " + String(heading_degree));

    String text[] = {"Richtung:", (String)heading_degree};
    MyDisplay::display_text(text, 2);

    Util::wait_interruptable(100);
}
