#include "Display.h"
Adafruit_SSD1306 MyDisplay::display;

void MyDisplay::initialize_display()
{
    display = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    // Bildschirm starten
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    display_text("System\nboot");
}
// Zeige Temperatur auf dem Display an
void MyDisplay::display_temperature(dht_results dht_measurement)
{
    display.clearDisplay();
    display.cp437(true); // Erlaubt ° Zeichen
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Temperatur\n");
    display.print(dht_measurement.temperature);

    // Setze Textgröße auf 1 und schreibe °
    display.setTextSize(1);
    display.write(167);
    display.setTextSize(2);

    display.print("C");
    display.display();
}

// Zeige Luftfeuchtigkeit auf dem Bildschirm an
void MyDisplay::display_humidity(dht_results dht_measurement)
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Luftf.\n");
    display.print(dht_measurement.humidity);
    display.print("%");
    display.display();
}
// Misst die Distanz und zeigt die Distanz auf dem Bildschirm an. Messung wird wiederholt bis eine gültige vorliegt.
void MyDisplay::display_distance(float distance)
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Distanz:\n");
    display.print(distance);
    display.print("cm");
    display.display();
}
static int dots = 0;
static int count_direction = 1;

// Bildschirmausgabe für NFC
void MyDisplay::display_NFC()
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("Scanne");
    for (int i = 0; i < dots; i++)
    {
        display.print(".");
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

    display.println("");
    display.display();
}

// Beliebigen text ausgeben
void MyDisplay::display_text(String text)
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print(text);
    display.display();
}

void MyDisplay::display_heartbeat(int pulse)
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.print("BPM:\n" + (String)pulse);
    display.display();
}

void MyDisplay::display_CO2(float ppm)
{
    {
        if (ppm <= LOW_CO2)
        {
            MyDisplay::display_text("Geringer CO2\nAnteil\n" + (String)ppm + "ppm");
        }
        else if (ppm <= NORMAL_CO2)
        {
            MyDisplay::display_text("Normaler CO2\nAnteil\n" + (String)ppm + "ppm");
        }
        else if (ppm <= HIGH_CO2)
        {
            MyDisplay::display_text("Hoher CO2\nAnteil\n" + (String)ppm + "ppm");
        }
        else
        {
            MyDisplay::display_text("Gas erkannt!\n" + (String)ppm + "ppm");
        }
    }
}