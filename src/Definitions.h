#ifndef DEFINITIONS_INCLUDED
#define DEFINITIONS_INCLUDED

// Anzahld der Module
#define NUMBER_OF_MODULES 6

// SDA und SCL Pins
#define SDA_PIN 21
#define SCL_PIN 22

// font for oled
#define FONT u8g2_font_timB14_tf

// DHT22
#define DHT_PIN 4
#define DHTTYPE DHT22 // DHT 11

// Potentiometer Pin
#define POTENTIO_PIN 34

// KY039 pin: Heartbeat
#define HEARTBEAT_PIN 36
#define heartbeat_samp_siz 4
#define heartbeat_rise_threshold 4

// MQ135
#define MQ_PIN 25

#define VERY_LOW_C02 0
#define LOW_CO2 300
#define NORMAL_CO2 450
#define HIGH_CO2 550
#define VERY_HIGH_CO2 1000
#define TOXIC_GAS 3000

#endif