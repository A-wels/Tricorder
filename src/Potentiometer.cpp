#include "Potentiometer.h"

// Task: Lese das Potentiometer dauerthaft aus. Wird in 2. Thread ausgeführt.
void MyPotentiometer::read_potentiometer(void *parameter)
{

    while (true)
    {
        Util::pot_val = analogRead(POTENTIO_PIN);
        Util::pot_val = map(Util::pot_val, 0, 4095, 0, NUMBER_OF_MODULES - 1); // Map auf die Anzahl der Module
        delay(100);
    }
}
void MyPotentiometer::intialize_potentiometer()
{
    // Pin für Potentiometer
    pinMode(POTENTIO_PIN, INPUT);

    // Task für Potentiometer polling
    TaskHandle_t xHandle = NULL;
    xTaskCreate(MyPotentiometer::read_potentiometer, "Read Potentiometer", 1000, NULL, 1, &xHandle);
}
