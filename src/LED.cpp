#include "LED.h"

void MyLED::initialize()
{

    pinMode(5, OUTPUT);
    pinMode(18, OUTPUT);
    pinMode(19, OUTPUT);

    digitalWrite(5, LOW);
    digitalWrite(18, LOW);
    digitalWrite(19, LOW);
    Serial.println("BLINK");
    blink_led(colors::RED, 400);
    blink_led(colors::YELLOW, 800, 4);
    blink_led(colors::GREEN, 1000, 8);
}
void MyLED::disable_all_leds()
{
    disable_led(RED);
    disable_led(GREEN);
    disable_led(YELLOW);
}
void MyLED::enable_led(colors LED)
{
    switch (LED)
    {
    case colors::RED:
        digitalWrite(PIN_RED, HIGH);
        break;
    case colors::YELLOW:
        digitalWrite(PIN_YELLOW, HIGH);
        break;
    case colors::GREEN:
        digitalWrite(PIN_GREEN, HIGH);
    }
}
void MyLED::disable_led(colors LED)
{
    switch (LED)
    {
    case colors::RED:
        digitalWrite(PIN_RED, LOW);
        break;
    case colors::YELLOW:
        digitalWrite(PIN_YELLOW, LOW);
        break;
    case colors::GREEN:
        digitalWrite(PIN_GREEN, LOW);
    }
}
void MyLED::blink_async(void *param)
{
    blink_struct b = *(blink_struct *)param;
    int pin;
    switch (b.color)
    {
    case colors::RED:
        pin = PIN_RED;
        break;
    case colors::YELLOW:
        pin = PIN_YELLOW;
        break;
    case colors::GREEN:
        pin = PIN_GREEN;
    }
    vTaskDelay(b.delay_ms);
    elapsedMillis since_enable;
    digitalWrite(pin, HIGH);
    while (since_enable < b.duration)
    {
        vTaskDelay(10);
    }
    digitalWrite(pin, LOW);
    vTaskDelete(NULL);
}
void MyLED::blink_led(colors LED, int duration, int delay_ms)
{
    blink_struct b;
    b.color = LED;
    b.duration = duration;
    b.delay_ms = delay_ms;
    disable_led(LED);

    TaskHandle_t xHandle = NULL;
    xTaskCreate(MyLED::blink_async, "Blink an LED", 1000, &b, 0, &xHandle);
    delay(4);
}

void MyLED::blink_distance(float distance)
{
    if (distance > 0 && distance <= 10)
    {
        disable_led(colors::GREEN);
        disable_led(colors::YELLOW);
        blink_led(colors::RED, 300);
    }
    else if (distance > 10 && distance <= 50)
    {
        disable_led(colors::GREEN);
        disable_led(colors::RED);
        enable_led(colors::YELLOW);
    }
    else if (distance > 50)
    {
        disable_led(colors::YELLOW);
        disable_led(colors::RED);
        enable_led(colors::GREEN);
    }
}