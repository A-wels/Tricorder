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
    delay(1000);
    blink_led(colors::RED, 300);
    blink_led(colors::YELLOW, 600);
    blink_led(colors::GREEN, 900);
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
    Serial.print("Duration: ");
    Serial.println(b.duration);
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
    elapsedMillis since_enable;
    Serial.print("Enable");
    Serial.println(b.color);
    digitalWrite(pin, HIGH);
    while (since_enable < b.duration)
    {
        vTaskDelay(10);
    }
    digitalWrite(pin, LOW);
    Serial.print("Disable");
    Serial.println(b.color);
    vTaskDelete(NULL);
}
void MyLED::blink_led(colors LED, int duration)
{
    blink_struct b;
    b.color = LED;
    b.duration = duration;
    disable_led(LED);

    TaskHandle_t xHandle = NULL;
    xTaskCreate(MyLED::blink_async, "Blink an LED", 1000, &b, 0, &xHandle);
    delay(10);
}