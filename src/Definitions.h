#ifndef DEFINITIONS_INCLUDED
#define DEFINITIONS_INCLUDED

// Anzahld der Module
#define NUMBER_OF_MODULES 5

// SDA und SCL Pins
#define SDA_PIN 21
#define SCL_PIN 22

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
#define MQ_PIN 39
#define RZERO 206.85

#define LOW_CO2 300
#define NORMAL_CO2 500
#define HIGH_CO2 2000

#endif