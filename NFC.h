#ifndef NFC_CLASS
#define NFC_CLASS

// Libraries for PN532
// https://github.com/elechouse/PN532
// Auf Basis von: https://warlord0blog.wordpress.com/2021/10/09/esp32-and-nfc-over-i2c/
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

#include <Arduino.h>

#include "Display.h"
#include "Util.h"

class MyNFC
{

public:
    static void connect_nfc(PN532 nfc, MyDisplay display);
    static void read_nfc(PN532 nfc, MyDisplay display);
    static bool nfc_connected;
};

#endif