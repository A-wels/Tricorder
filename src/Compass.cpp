#include "Compass.h"

// Kompass an I2C Adresse zuweisen
QMC5883LCompass Compass::compass;
void Compass::intialize()
{
    compass.init();
}

// Kompass auslesen
void Compass::get_readings()
{
    // a: Azimuth, b: Bearing
    int a, b;
    char directions[3];

    int val_at_start = Util::pot_val;
    String text[] = {"Starte", "Kompass"};
    MyDisplay::display_text(text, 2);

    // Solange das Kompassmodul ausgewählt ist
    while (val_at_start == Util::pot_val)
    {
        // Kompass muss jedes mal neu initialisiert werden. Probleme im Zusammenspiel mit dem Bildschirm
        compass.init();
        compass.read();
        a = compass.getAzimuth();
        compass.getDirection(directions, a);

        // Blinken + Bildschirmausgabe
        MyLED::blink_led(MyLED::colors::GREEN, 200);
        MyDisplay::display_compass(a);

        Util::wait_interruptable(150);
    }
}
