#include "Display.h"
U8G2_SH1106_128X64_NONAME_1_SW_I2C MyDisplay::display(U8G2_R0, SCL_PIN, SDA_PIN);

void MyDisplay::initialize_display()
{
    // Bildschirm starten
    display.begin();
    String text[] = {"System", "Boot"};
    display_text(text,2);
}
// Zeige Temperatur auf dem Display an
void MyDisplay::display_temperature(dht_results dht_measurement)
{
    // display.clearDisplay();
    // display.cp437(true); // Erlaubt ° Zeichen
    // display.setTextSize(2);
    // display.setTextColor(WHITE);
    // display.setCursor(0, 0);
    // display.print("Temperatur\n");
    // display.print(dht_measurement.temperature);

    // // Setze Textgröße auf 1 und schreibe °
    // display.setTextSize(1);
    // display.write(167);
    // display.setTextSize(2);

    // display.print("C");
    // display.display();
}

// Zeige Luftfeuchtigkeit auf dem Bildschirm an
void MyDisplay::display_humidity(dht_results dht_measurement)
{
    // display.clearDisplay();
    // display.setTextSize(2);
    // display.setTextColor(WHITE);
    // display.setCursor(0, 0);
    // display.print("Luftf.\n");
    // display.print(dht_measurement.humidity);
    // display.print("%");
    // display.display();
}
// Misst die Distanz und zeigt die Distanz auf dem Bildschirm an. Messung wird wiederholt bis eine gültige vorliegt.
void MyDisplay::display_distance(float distance)
{
    // display.clearDisplay();
    // display.setTextSize(2);
    // display.setTextColor(WHITE);
    // display.setCursor(0, 0);
    // display.print("Distanz:\n");
    // display.print(distance);
    // display.print("cm");
    // display.display();
}
static int dots = 0;
static int count_direction = 1;

// Bildschirmausgabe für NFC
void MyDisplay::display_NFC()
{
    // display.clearDisplay();
    // display.setTextSize(2);
    // display.setTextColor(WHITE);
    // display.setCursor(0, 0);
    // display.print("Scanne");
    // for (int i = 0; i < dots; i++)
    // {
    //     display.print(".");
    // }
    // if (dots == 0)
    // {
    //     count_direction = 1;
    // }
    // else if (dots == 3)
    // {
    //     count_direction = -1;
    // }
    // dots = dots + count_direction;

    // display.println("");
    // display.display();
}


// Beliebigen text ausgeben
void MyDisplay::display_text(String text[], int lines)
{

 
    display.firstPage();
    do
    {
        display.setFont(u8g2_font_ncenB14_tr);
        for(int i=1;i<lines+1;i++){
            display.drawStr(0, 15*i, text[i-1].c_str());

        }

    } while (display.nextPage());

    // display.clearDisplay();
    // display.setTextSize(2);
    // display.setTextColor(WHITE);
    // display.setCursor(0, 0);
    // display.print(text);
    // display.display();
}

void MyDisplay::display_heartbeat(int pulse)
{
    // display.clearDisplay();
    // display.setTextSize(2);
    // display.setTextColor(WHITE);
    // display.setCursor(0, 0);
    // display.print("BPM:\n" + (String)pulse);
    // display.display();
}

void MyDisplay::display_CO2(float ppm)
{
    // {
    //     if (ppm <= LOW_CO2)
    //     {
    //         MyDisplay::display_text("Geringer CO2\nAnteil\n" + (String)ppm + "ppm");
    //     }
    //     else if (ppm <= NORMAL_CO2)
    //     {
    //         MyDisplay::display_text("Normaler CO2\nAnteil\n" + (String)ppm + "ppm");
    //     }
    //     else if (ppm <= HIGH_CO2)
    //     {
    //         MyDisplay::display_text("Hoher CO2\nAnteil\n" + (String)ppm + "ppm");
    //     }
    //     else
    //     {
    //         MyDisplay::display_text("Gas erkannt!\n" + (String)ppm + "ppm");
    //     }
    // }
}