#ifndef ULTRASCHALL_CLASS
#define ULTRASCHALL_CLASS
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

// Ultraschall Pins
#define TRIG_PIN 14
#define ECHO_PIN 13

class MyUltraschall
{
public:
    void initialize();
    float measure_distance();
};
#endif