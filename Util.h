#ifndef UTIL_CLASS
#define UTIL_CLASS
#include <Arduino.h>
class Util
{
public:
    static int pot_val;
    static void wait_interruptable(int duration);
};
#endif