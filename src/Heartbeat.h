#ifndef HEARTBEAT_CLASS
#define HEARTBEAT_CLASS
#include "Definitions.h"
#include "Util.h"
#include "Display.h"
// Adapted from: https://create.arduino.cc/projecthub/Johan_Ha/from-ky-039-to-heart-rate-0abfca
class Heartbeat
{

public:
    static void measure_heartbeat();
};
#endif