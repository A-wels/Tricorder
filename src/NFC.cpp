#include "NFC.h"
bool MyNFC::nfc_connected = false;

PN532_I2C MyNFC::pn532i2c(Wire);
PN532 MyNFC::nfc(pn532i2c);

void MyNFC::connect_nfc()
{
    if (!MyNFC::nfc_connected)
    {
        MyNFC::nfc.begin();
    }
    // Serial.println("Connecting to nfc");
    //  Connected, show version
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata)
    {
        //  String text[] = {"Scanner", " defekt"};
        //  MyDisplay::display_text(text, 2);
        //       Serial.println("PN53x card not found!");
        MyNFC::nfc_connected = false;
    }
    else
    {

        // port
        Serial.print("Found chip PN5");
        Serial.println((versiondata >> 24) & 0xFF, HEX);
        Serial.print("Firmware version: ");
        Serial.print((versiondata >> 16) & 0xFF, DEC);
        Serial.print('.');
        Serial.println((versiondata >> 8) & 0xFF, DEC);

        // Set the max number of retry attempts to read from a card
        // This prevents us from waiting forever for a card, which is
        // the default behaviour of the PN532.
        nfc.setPassiveActivationRetries(0xFF);

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
    boolean success;
    // Buffer: Hier wird die UID des NFC Chips gespeichert
    uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};

    // UID Länge
    uint8_t uid_length;

    // Stelle eine Verbindung her
    String text[] = {"Starte", "Scanner..."};
    MyDisplay::display_text(text, 2);

    Util::wait_interruptable(1000);

    while (task_at_start == Util::pot_val)
    {

        // Stelle eine Verbindung mit dem NFC Modul her
        while (!MyNFC::nfc_connected)
        {
            connect_nfc();
        }
        Serial.println("Connected");

        if (Util::pot_val != task_at_start)
        {
            break;
        }

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
