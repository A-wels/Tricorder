#ifndef POTENTIOMETER_CLASS
#define POTENTIOMETER_CLASS
#include "Util.h"
#include "Definitions.h"

class MyPotentiometer
{
public:
    static void intialize_potentiometer();

    // Task: Lese das Potentiometer dauerthaft aus. Wird in 2. Thread ausgeführt.
    static void read_potentiometer(void *parameter);
};
#endif