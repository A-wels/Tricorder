#ifndef DISPLAY_CLASS
#define DISPLAY_CLASS

#include <Adafruit_GFX.h>
#include <U8g2lib.h>
#include "Temperature.h"
#include "Util.h"
#include "Definitions.h"
// Display
#define OLED_RESET -1

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
class MyDisplay
{
public:
    static void initialize_display();
    static void display_temperature(dht_results dht_measurement);
    static void display_humidity(dht_results dht_measurement);
    static void display_distance(float distance);
    static void display_NFC();
    static void display_text(String text[], int lines);
    static void display_heartbeat(int pulse);
    static void display_CO2(float ppm);
    static U8G2_SH1106_128X64_NONAME_1_SW_I2C display;
};
#endif
