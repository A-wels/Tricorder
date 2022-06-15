#include "Util.h"
// Wartet ungefähr für die angegebene Dauer. Wird vorzeitig abgebrochen, wenn pot_val sich verändert.
int Util::pot_val = 0;

void Util::wait_interruptable(int duration)
{

    int time_start = millis();
    int pot_at_start = Util::pot_val;
    while (millis() - time_start < duration && Util::pot_val == pot_at_start)
    {
        delay(100);
    }
    return;
}