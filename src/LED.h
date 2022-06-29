#ifndef LED_CLASS
#define LED_CLASS
#include <elapsedMillis.h>
#include "Util.h"
#include "Definitions.h"
class MyLED
{

public:
    enum colors
    {
        RED,
        GREEN,
        YELLOW
    };
    struct blink_struct
    {
        colors color;
        int duration;
    };

    static void initialize();
    static void enable_led(colors LED);
    static void blink_led(colors LED, int duration);
    static void disable_led(colors LED);

private:
    static void blink_async(void *param);
};
#endif