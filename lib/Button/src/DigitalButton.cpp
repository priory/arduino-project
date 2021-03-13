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
    if (digitalRead(this->getPin())) {
        this->_press(micro);
    } else {
        this->_release(micro);
    }

    this->_handleEvents();
}
