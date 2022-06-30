#ifndef ULTRASCHALL_CLASS
#define ULTRASCHALL_CLASS
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Definitions.h"
class MyUltraschall
{
public:
    void initialize();
    float measure_distance();
};
#endif