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
    display.firstPage();
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
    }
    if (ppm <= NORMAL_CO2)
    {
        text[0] = "Geringer CO2";
        text[1] = "Anteil";
        text[2] = (String)percent + "%";
    }
    else if (ppm <= HIGH_CO2)
    {
        text[0] = "Normaler CO2";
        text[1] = "Anteil";
        text[2] = (String)percent + "%";
    }
    else if (ppm <= VERY_HIGH_CO2)
    {
        text[0] = "Hoher CO2";
        text[1] = "Anteil";
        text[2] = (String)percent + "%";
    }
    else if (ppm <= TOXIC_GAS)
    {
        text[0] = "Zu hoher C02";
        text[1] = "Anteil erkannt!";
        text[2] = (String)percent + "%";
    }
    else
    {
        text[0] = "Gefaehrliches";
        text[1] = "Gas erkannt!";
        text[2] = "GEFAHR";
    }

    display_text(text, 3);
}

void MyDisplay::display_NFC_results(uint8_t uid)
{
    String text[3];
    uint8_t BABY_YODA[] = {4, 140, 196, 146, 243, 103, 128};

    if (uid == *BABY_YODA)
    {
        Serial.println("YAY");
        text[0] = "Lebensform aus";
        text[1] = "Star Wards";
        text[2] = "Name: Grogu";

        display_text(text, 3);
        Util::wait_interruptable(5000);
    }
}