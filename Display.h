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
    MyDisplay();
    void initialize_display();
    void display_temperature(dht_results dht_measurement);
    void display_humidity(dht_results dht_measurement);
    void display_distance(float distance);
    void display_NFC();
    void display_text(String text);

private:
    Adafruit_SSD1306 display;
};

#endif
