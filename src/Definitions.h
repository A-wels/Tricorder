#ifndef DEFINITIONS_INCLUDED
#define DEFINITIONS_INCLUDED

// Anzahld der Module
#define NUMBER_OF_MODULES 6

// SDA und SCL Pins
#define SDA_PIN 21
#define SCL_PIN 22

// font for oled
#define FONT u8g2_font_timB14_tf

// LED PINS
#define PIN_RED 19
#define PIN_YELLOW 5
#define PIN_GREEN 18

// Ultraschall Pins
#define TRIG_PIN 14
#define ECHO_PIN 13

// DHT22
#define DHT_PIN 4
#define DHTTYPE DHT22 // DHT 11

// Potentiometer Pin
#define POTENTIO_PIN 34

// KY039 pin: Heartbeat
#define HEARTBEAT_PIN 36

// MQ135
#define MQ_PIN 39

#define VERY_LOW_C02 0
#define LOW_CO2 300
#define NORMAL_CO2 450
#define HIGH_CO2 550
#define VERY_HIGH_CO2 1000
#define TOXIC_GAS 3000

// Kompass:

// Magnetic declination ausgleich: https://ngdc.noaa.gov/geomag/declination.shtml
#define MAGNETIC_DECLINATION 0.0349

#endif