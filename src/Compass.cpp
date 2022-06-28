#include "Compass.h"

// Kompass an I2C Adresse zuweisen

void Compass::intialize()
{

    if (!Compass::compass.begin())
    {
        String text[] = {"Kompass", "Defekt"};
        MyDisplay::display_text(text, 2);
        Serial.println("HMC5388 nicht erkannt");
        Util::wait_interruptable(10000);
    }
}

void Compass::get_readings()
{
    // Documentation: https://github.com/DFRobot/DFRobot_QMC5883
    /**
     * @brief  Set declination angle on your location and fix heading
     * @n      You can find your declination on: http://magnetic-declination.com/
     * @n      (+) Positive or (-) for negative
     * @n      For Münster / Germany declination angle is 2'58E (positive)
     * @n      Formula: (deg + (min / 60.0)) / (180 / PI);
     */
    // float declinationAngle = (2.0 + (58.0 / 60.0)) / (180 / PI);
    //  compass.setDeclinationAngle(declinationAngle);
    sVector_t mag = compass.readRaw();
    compass.getHeadingDegrees();
    Serial.print("X:");
    Serial.print(mag.XAxis);
    Serial.print(" Y:");
    Serial.print(mag.YAxis);
    Serial.print(" Z:");
    Serial.println(mag.ZAxis);
    Serial.print("Degress = ");
    Serial.println(mag.HeadingDegress);

    Serial.println("Heading: " + (String)mag.HeadingDegress);

    String text[] = {"Richtung:", (String)mag.HeadingDegress};
    MyDisplay::display_text(text, 2);

    Util::wait_interruptable(100);
}
