#include "Display.h"
U8G2_SH1106_128X64_NONAME_F_SW_I2C MyDisplay::display(U8G2_R0, SCL_PIN, SDA_PIN);

void MyDisplay::initialize_display()
{
    // Bildschirm starten
    display.begin();
    display.enableUTF8Print();
    String text[] = {"System", "Boot"};
    display_text(text, 2);
}
// Zeige Temperatur auf dem Display an
void MyDisplay::display_temperature(dht_results dht_measurement)
{
    String text[] = {
        "Temperatur:",
        (String)dht_measurement.temperature + "\xb0" + "C",
    };
    display_text(text, 2);
}

// Zeige Luftfeuchtigkeit auf dem Bildschirm an
void MyDisplay::display_humidity(dht_results dht_measurement)
{
    String text[] = {"Luftf.:", (String)dht_measurement.humidity + "%"};
    display_text(text, 2);
}
// Misst die Distanz und zeigt die Distanz auf dem Bildschirm an. Messung wird wiederholt bis eine gültige vorliegt.
void MyDisplay::display_distance(float distance)
{
    String text[] = {
        "Distanz:",
        (String)distance + "cm"};
    display_text(text, 2);
}
static int dots = 0;
static int count_direction = 1;

// Bildschirmausgabe für NFC
void MyDisplay::display_NFC()
{
    String t = "Scanne";

    for (int i = 0; i < dots; i++)
    {
        t = t + ".";
    }
    if (dots == 0)
    {
        count_direction = 1;
    }
    else if (dots == 3)
    {
        count_direction = -1;
    }
    dots = dots + count_direction;
    String text[] = {t};

    display_text(text, 1);
}

// Beliebigen text ausgeben
void MyDisplay::display_text(String text[], int lines)
{

    display.clearBuffer();
    display.setFont(FONT);
    for (int i = 1; i < lines + 1; i++)
    {
        display.drawStr(0, 15 * i, text[i - 1].c_str());
    }
    display.sendBuffer();
}

void MyDisplay::display_heartbeat(int pulse)
{
    String text[] = {"BPM: " + (String)pulse};
    Serial.println(text[0]);
    display_text(text, 1);
}

void MyDisplay::display_CO2(float percent, float ppm)
{
    String text[3];
    if (ppm <= LOW_CO2)
    {
        text[0] = "Minimaler CO2";
        text[1] = "Anteil";
        text[2] = (String)percent + "%";
        MyLED::disable_led(MyLED::colors::RED);
        MyLED::disable_led(MyLED::colors::YELLOW);
        MyLED::enable_led(MyLED::colors::GREEN);
    }
    if (ppm <= NORMAL_CO2)
    {
        text[0] = "Geringer CO2";
        text[1] = "Anteil";
        text[2] = (String)percent + "%";
        MyLED::disable_led(MyLED::colors::RED);
        MyLED::disable_led(MyLED::colors::YELLOW);
        MyLED::enable_led(MyLED::colors::GREEN);
    }
    else if (ppm <= HIGH_CO2)
    {
        text[0] = "Normaler CO2";
        text[1] = "Anteil";
        text[2] = (String)percent + "%";
        MyLED::disable_led(MyLED::colors::RED);
        MyLED::enable_led(MyLED::colors::YELLOW);
        MyLED::disable_led(MyLED::colors::GREEN);
    }
    else if (ppm <= VERY_HIGH_CO2)
    {
        text[0] = "Hoher CO2";
        text[1] = "Anteil";
        text[2] = (String)percent + "%";
        MyLED::disable_led(MyLED::colors::RED);
        MyLED::enable_led(MyLED::colors::YELLOW);
        MyLED::disable_led(MyLED::colors::GREEN);
    }
    else if (ppm <= TOXIC_GAS)
    {
        text[0] = "Zu hoher C02";
        text[1] = "Anteil erkannt!";
        text[2] = (String)percent + "%";
        MyLED::disable_led(MyLED::colors::RED);
        MyLED::disable_led(MyLED::colors::YELLOW);
        MyLED::enable_led(MyLED::colors::RED);
    }
    else
    {
        text[0] = "Gefaehrliches";
        text[1] = "Gas erkannt!";
        text[2] = "GEFAHR";
        MyLED::disable_led(MyLED::colors::RED);
        MyLED::disable_led(MyLED::colors::YELLOW);
        MyLED::blink_led(MyLED::colors::RED, 200);
    }

    display_text(text, 3);
}

void MyDisplay::display_NFC_results(uint8_t uid[])
{
    String text[3];
    uint8_t BABY_YODA[] = {4, 140, 196, 146, 243, 103, 128};
    uint8_t PARFUEM[] = {4, 128, 209, 146, 243, 103, 128};

    if (uid[1] == BABY_YODA[1] && uid[2] == BABY_YODA[2])
    {
        text[0] = "Name: Grogu";
        text[1] = "Art: Kuscheltier";
        text[2] = "Lebendig: Nein";

        display_text(text, 3);
        Util::wait_interruptable(5000);
    }
    else if (uid[1] == PARFUEM[1] && uid[2] == PARFUEM[2])
    {
        text[0] = "Name: Parfuem";
        text[1] = "Ueberdeckt Gestank";
        text[2] = "Enthaelt: Alkohol";

        display_text(text, 3);
        Util::wait_interruptable(5000);
    }
}

// https://www.reddit.com/r/arduino/comments/9mg45n/arduino_compass_display_or_why_i_relearned_8th/
void MyDisplay::display_compass(float heading)
{
    int bearing = (int)heading;
    display.clearBuffer();
    static int armLength = 22;
    static int arrowLength = 15;
    static int cx = 86;
    static int cy = 32;
    int armX, armY, arrow1X, arrow1Y, arrow2X, arrow2Y;

    // convert degree to radian
    float bearingRad = bearing / 57.2957795;     // 1 Radian is 57.2957 degrees
    float arm1Rad = (bearing - 15) / 57.2957795; // calculate for little arrow arms +/- a few degrees.
    float arm2Rad = (bearing + 15) / 57.2957795;

    armX = armLength * cos(bearingRad);  // use trig to get x and y values. x=hypotenuse*cos(angle in Rads)
    armY = -armLength * sin(bearingRad); // y = hypotenuse*sin(angle in Rads)

    arrow1X = arrowLength * cos(arm1Rad); // x and y offsets to draw the arrow bits
    arrow1Y = -arrowLength * sin(arm1Rad);
    arrow2X = arrowLength * cos(arm2Rad); // x and y offsets to draw the rest of the arrow bits
    arrow2Y = -arrowLength * sin(arm2Rad);

    // draw line, circle, and arrows
    display.drawLine(cx, cy, cx - armY, cy - armX); // for some reason have to invert x and y to get correct compass heading
    // u8g2.drawLine(cx-armY, cy-armX, cx-arrow1Y, cy-arrow1X); // draw 1/2 of arrowhead
    // u8g2.drawLine(cx-armY, cy-armX, cx-arrow2Y, cy-arrow2X);
    display.drawTriangle(cx - armY, cy - armX, cx - arrow1Y, cy - arrow1X, cx - arrow2Y, cy - arrow2X);
    display.drawCircle(cx, cy, armLength, U8G2_DRAW_ALL);
    display.drawCircle(cx, cy, 2, U8G2_DRAW_ALL);

    // Draw tick marks at each Compass point
    display.drawLine(cx, cy - (armLength - 2), cx, cy - (armLength + 2)); // North tick mark
    display.drawLine(cx, cy + (armLength - 2), cx, cy + (armLength + 2)); // South tick mark
    display.drawLine(cx - (armLength - 2), cy, cx - (armLength + 2), cy); // West tick mark
    display.drawLine(cx + (armLength - 2), cy, cx + (armLength + 2), cy); // East tick mark
    // u8g2.setFont(u8g_font_unifont);
    display.setFont(u8g2_font_profont12_tf); // 8 pixel font

    display.setCursor(10, 15);
    display.print("Richtung: ");
    // Label the Compass Directions
    display.setCursor(cx - 2, cy - (armLength + 1)); // need to offset each Label by a bit. - See u8g2.print() function
    display.print("N");
    display.setCursor(cx - 2, cy + (armLength + 10)); // 2 + font size of 8 = 10
    display.print("S");
    display.setCursor(cx + (armLength + 4), cy + 4);
    display.print("E");
    display.setCursor(cx - (armLength + 8), cy + 4);
    display.print("W");

    // Display the actual bearing in a larger font
    display.setCursor(20, 35);
    display.setFont(u8g2_font_timR14_tf); // separate font for Bearing
    display.print((int)heading);

    display.sendBuffer();
}
