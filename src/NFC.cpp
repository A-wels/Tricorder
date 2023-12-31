#include "NFC.h"
bool MyNFC::nfc_connected = false;

PN532_I2C MyNFC::pn532i2c(Wire);
PN532 MyNFC::nfc(pn532i2c);

// Verbindung mit dem NFC Modul aufbauen
void MyNFC::connect_nfc()
{
    if (!MyNFC::nfc_connected)
    {
        MyNFC::nfc.begin();
    }
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata)
    {
        Serial.println("NOPE");
        MyNFC::nfc_connected = false;
    }
    else
    {
        //  Connected, show version

        Serial.print("Found chip PN5");
        Serial.println((versiondata >> 24) & 0xFF, HEX);
        Serial.print("Firmware version: ");
        Serial.print((versiondata >> 16) & 0xFF, DEC);
        Serial.print('.');
        Serial.println((versiondata >> 8) & 0xFF, DEC);

        // Set the max number of retry attempts to read from a card
        // This prevents us from waiting forever for a card, which is
        // the default behaviour of the PN532.
        nfc.setPassiveActivationRetries(0x0a);

        // configure board to read RFID tags
        nfc.SAMConfig();

        Serial.println("Waiting for card (ISO14443A)...");
        Serial.println("");

        MyNFC::nfc_connected = true;
    }
}

// Lese einen NFC Chip mit dem PN532 (NFC Reader) aus
void MyNFC::read_nfc()
{

    // Wenn Util::pot_val abweicht, so wurde die aktuelle Aufgabe geändert
    int task_at_start = Util::pot_val;

    // Stelle eine Verbindung her
    String text[] = {"Starte", "Scanner..."};
    MyDisplay::display_text(text, 2);

    MyLED::blink_led(MyLED::colors::RED, 200, 300);
    MyLED::blink_led(MyLED::colors::YELLOW, 200, 600);
    MyLED::blink_led(MyLED::colors::GREEN, 200, 900);
    MyLED::blink_led(MyLED::colors::GREEN, 200, 1100);
    MyLED::blink_led(MyLED::colors::YELLOW, 200, 1300);
    MyLED::blink_led(MyLED::colors::RED, 200, 1500);
    MyLED::blink_led(MyLED::colors::YELLOW, 200, 1700);
    MyLED::blink_led(MyLED::colors::GREEN, 200, 1900);
    Util::wait_interruptable(1000);
    while (task_at_start == Util::pot_val)
    {

        // Versuche einen NFC Chip auszulesen
        MyDisplay::display_NFC();

        // Erfolgreicher scan?
        boolean success;
        // Buffer to store the UID
        uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
        // UID size (4 or 7 bytes depending on card type)
        uint8_t uidLength;

        // Scan twice. If I do it once, the scan always fails. Can't be bothered right now
        MyNFC::nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
        success = MyNFC::nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

        // Falls erkannt: Datenausgabe
        if (success)
        {
            MyLED::blink_led(MyLED::colors::GREEN, 4000);
            String text_success[] = {"Scan", "erfolgreich!"};
            MyDisplay::display_text(text_success, 2);

            Serial.println("Karte erkannt");
            Serial.print("Size of UID: ");
            Serial.print(uidLength, DEC);
            Serial.println(" bytes");
            Serial.print("UID: ");
            for (uint8_t i = 0; i < uidLength; i++)
            {
                Serial.print(" ");
                Serial.print(uid[i]);
            }
            Serial.println("");
            Serial.println("");

            Util::wait_interruptable(1000);
            MyDisplay::display_NFC_results(uid);
        }
        else
        {
            // PN532 probably timed out waiting for a card
            Serial.println("Timed out waiting for a card");
        }
    }
}
