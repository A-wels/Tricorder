#include "Display.h"
U8G2_SH1106_128X64_NONAME_1_SW_I2C MyDisplay::display(U8G2_R0, SCL_PIN, SDA_PIN);

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

    display.firstPage();
    do
    {
        display.setFont(FONT);
        for (int i = 1; i < lines + 1; i++)
        {
            display.drawStr(0, 15 * i, text[i - 1].c_str());
        }

    } while (display.nextPage());
}

void MyDisplay::display_heartbeat(int pulse)
{
    String text[] = {"BPM: " + (String)pulse};
    Serial.println(text[0]);
    display_text(text, 1);
}

void MyDisplay::display_CO2(float ppm)
{
    String text[3];
    if (ppm <= LOW_CO2)
    {
        text[0] = "Geringer CO2";
        text[1] = "Anteil";
        text[2] = (String)ppm + "ppm";
    }
    else if (ppm <= NORMAL_CO2)
    {
        text[0] = "Normaler CO2";
        text[1] = "Anteil";
        text[2] = (String)ppm + "ppm";
    }
    else if (ppm <= HIGH_CO2)
    {
        text[0] = "Hoher CO2";
        text[1] = "Anteil";
        text[2] = (String)ppm + "ppm";
    }
    else
    {
        text[0] = "Gefährliches Gas";
        text[1] = "erkannt!";
        text[2] = (String)ppm + "ppm";
    }

    display_text(text, 3);
}