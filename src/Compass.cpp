#include "Compass.h"

// Kompass an I2C Adresse zuweisen
QMC5883LCompass Compass::compass;
void Compass::intialize()
{
    compass.init();
}

void Compass::get_readings()
{
    int x, y, z, a, b;
    char directions[3];

    int val_at_start = Util::pot_val;
    String text[] = {"Starte", "Kompass"};
    MyDisplay::display_text(text, 2);

    while (val_at_start == Util::pot_val)
    {
        compass.init();
        compass.read();
        x = compass.getX();
        y = compass.getY();
        z = compass.getZ();
        a = compass.getAzimuth();

        compass.getDirection(directions, a);
        MyLED::blink_led(MyLED::colors::GREEN, 200);

        MyDisplay::display_compass(a);

        Util::wait_interruptable(150);
    }
}
