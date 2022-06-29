#ifndef COMPASS_CLASS
#define COMPASS_CLASS

#include <QMC5883LCompass.h>

#include "Display.h"
#include "Util.h"
#include "Definitions.h"
#include "NFC.h"
#include "LED.h"

class Compass
{
public:
    static QMC5883LCompass compass;

    static void intialize();
    static void get_readings();
};

#endif