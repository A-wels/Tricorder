#ifndef DISPLAY_CLASS
#define DISPLAY_CLASS

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Temperature.h"
#include "Util.h"

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
    static void display_text(String text);
    static void display_heartbeat(int pulse);
    static void display_CO2(float ppm);
    static Adafruit_SSD1306 display;

private:
};
#endif
