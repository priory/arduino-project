#include <Arduino.h>

#ifndef DIGITALBUTTON_H

#include "DigitalButton.h"

#endif

DigitalButton::DigitalButton(
        uint8_t pin,
        unsigned long int bounce,
        uint8_t mode
) : AbstractButton(pin, bounce) {
    pinMode(pin, mode);
}

void DigitalButton::loop(const unsigned long &micro) {
    if (digitalRead(getPin())) {
        _press(micro);
    } else {
        _release(micro);
    }

    _handleEvents();
}
