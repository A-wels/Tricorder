#ifndef COMPASS_CLASS
#define COMPASS_CLASS

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include "Display.h"
#include "Util.h"
#include "Definitions.h"

class Compass
{
public:
    static Adafruit_HMC5883_Unified compass;

    static void intialize();
    static void get_readings();
};

#endif