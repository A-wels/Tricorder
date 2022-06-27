
#ifndef CO2_CLASS
#define CO2_CLASS

#include <MQ135.h>
#include "Definitions.h"
#include "Util.h"
#include "Display.h"

class GasSensor
{
public:
    static void intialize_sensor();
    static void get_ppm();
    static void get_gas_level(float percent);
    static MQ135 gas_sensor;
};
#endif