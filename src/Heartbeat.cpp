// Adapted from: https://create.arduino.cc/projecthub/Johan_Ha/from-ky-039-to-heart-rate-0abfca
#include "Heartbeat.h"

// Herzschlag messen. Funktioniert aufgrund des Sensors eher weniger.
void Heartbeat::measure_heartbeat()
{
    MyLED::disable_all_leds();
    // get current task id
    int task_at_start = Util::pot_val;

    float reads[4], sum;
    long int now, ptr;
    float last, reader, start;
    float first, second, third, before, print_value;
    bool rising;
    int rise_count;
    int n;
    long int last_beat;

    for (int i = 0; i < 4; i++)
        reads[i] = 0;
    sum = 0;
    ptr = 0;
    String text[2];

    // Repeat until a different module was selected
    while (task_at_start == Util::pot_val)
    {
        // calculate an average of the sensor
        // during a 20 ms period (this will eliminate
        // the 50 Hz noise caused by electric light
        n = 0;
        start = millis();
        reader = 0.;
        do
        {
            reader += analogRead(HEARTBEAT_PIN);
            n++;
            now = millis();
        } while (now < start + 20);
        reader /= n; // we got an average

        // Add the newest measurement to an array
        // and subtract the oldest measurement from the array
        // to maintain a sum of last measurements
        sum -= reads[ptr];
        sum += reader;
        reads[ptr] = reader;
        last = sum / 4;
        // now last holds the average of the values in the array

        // check for a rising curve (= a heart beat)
        if (last > before)
        {
            rise_count++;
            if (!rising && rise_count > 5)
            {
                // Ok, we have detected a rising curve, which implies a heartbeat.
                // Record the time since last beat, keep track of the two previous
                // times (first, second, third) to get a weighed average.
                // The rising flag prevents us from detecting the same rise more than once.
                rising = true;
                first = millis() - last_beat;
                last_beat = millis();

                // Calculate the weighed average of heartbeat rate
                // according to the three last beats
                print_value = 60000. / (0.4 * first + 0.3 * second + 0.3 * third);
                text[0] = "BPM: " + (String)(int)print_value;
                text[1] = "";
                MyDisplay::display_text(text, 2);
                MyLED::enable_led(MyLED::colors::RED);
                third = second;
                second = first;
            }
        }
        else
        {
            // Ok, the curve is falling
            rising = false;
            MyLED::disable_led(MyLED::colors::RED);

            rise_count = 0;
        }
        before = last;

        ptr++;
        ptr %= 4;
    }
}