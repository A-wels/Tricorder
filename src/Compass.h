#ifndef COMPASS_CLASS
#define COMPASS_CLASS

#include <DFRobot_QMC5883.h>

#include "Display.h"
#include "Util.h"
#include "Definitions.h"

class Compass
{
public:
    static DFRobot_QMC5883 compass;

    static void intialize();
    static void get_readings();
};
DFRobot_QMC5883 Compass::compass(&Wire, 0x0D);

#endif