#include "NFC.h"
bool MyNFC::nfc_connected = false;

void MyNFC::connect_nfc(PN532 nfc, MyDisplay display)
{
    nfc.begin();
    // Connected, show version
    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata)
    {
        display.display_text("Scanner\n defekt.");
        Serial.println("PN53x card not found!");
        MyNFC::nfc_connected = false;
    }

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

    Serial.println("Waiting for card (ISO14443A Mifare)...");
    Serial.println("");

    MyNFC::nfc_connected = true;
}

// Lese einen NFC Chip mit dem PN532 (NFC Reader) aus
void MyNFC::read_nfc(PN532 nfc, MyDisplay display)
{
    return;

    // Wenn Util::pot_val abweicht, so wurde die aktuelle Aufgabe geändert
    int task_at_start = Util::pot_val;
    boolean success;
    // Buffer: Hier wird die UID des NFC Chips gespeichert
    int *uid_ptr = (int *)malloc(7 * sizeof(uint8_t));

    for (int i = 0; i < 7; i++)
    {
        uid_ptr[i] = (uint8_t)0;
    }
    free(uid_ptr);
    // UID Länge
    uint8_t uid_length;

    // Stelle eine Verbindung
    MyNFC::nfc_connected = false;

    display.display_text("Starte\nScanner...");

    Util::wait_interruptable(500);

    while (task_at_start == Util::pot_val)
    {
        static boolean success;
        // Buffer to store the UID
        static uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};

        // UID size (4 or 7 bytes depending on card type)
        static uint8_t uidLength;
        // Stelle eine Verbindung mit dem NFC Modul her
        while (!MyNFC::nfc_connected && task_at_start == Util::pot_val)
        {
            connect_nfc(nfc, display);
        }

        if (Util::pot_val != task_at_start)
        {
            break;
        }

        // Versuche einen NFC Chip auszulesen
        display.display_NFC();

        success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

        // Falls erkannt: Datenausgabe
        if (success)
        {
            display.display_text("Scan\nerfolgreich");

            Serial.println("Card Detected");
            Serial.print("Size of UID: ");
            Serial.print(uidLength, DEC);
            Serial.println(" bytes");
            Serial.print("UID: ");
            for (uint8_t i = 0; i < uidLength; i++)
            {
                Serial.print(" 0x");
                Serial.print(uid[i], HEX);
            }
            Serial.println("");
            Serial.println("");

            Util::wait_interruptable(1000);
        }
        else
        {
            // PN532 probably timed out waiting for a card
            Serial.println("Timed out waiting for a card");
        }
        if (Util::pot_val == task_at_start)
        {
            //  connect_nfc(nfc, display);
        }
    }
    Serial.println("Checking for errors");
    bool working = heap_caps_check_integrity_all(true);
    Serial.println(working);
    Serial.println("Check done");
    delay(1000);
    free(uid_ptr);
}
