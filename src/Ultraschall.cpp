#include "Ultraschall.h"

// Intialisierung
void MyUltraschall::initialize()
{
    // Pins für Ultraschall
    pinMode(TRIG_PIN, OUTPUT); // Setzt den vorher definierten trigPin als Output.
    pinMode(ECHO_PIN, INPUT);  // Setzt den vorher defnierten echoPin als Input.
}

// Miss die Distanz.
float MyUltraschall::measure_distance()
{

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    float duration_us = pulseIn(ECHO_PIN, HIGH);
    return 0.017 * duration_us;
}