#include <Arduino.h>

#ifndef INTERRUPTINGBUTTON_H

#include "InterruptingButton.h"

#endif

InterruptingButton::InterruptingButton(
        uint8_t pin,
        void (*userFunc)(),
        unsigned long int bounce
) : AbstractButton(pin, bounce) {
    attachInterrupt(digitalPinToInterrupt(this->_pin), userFunc, CHANGE);
}

InterruptingButton::~InterruptingButton() {
    detachInterrupt(digitalPinToInterrupt(this->_pin));
};

void InterruptingButton::interruptHandle() {
    if (digitalRead(this->getPin())) {
        this->state = true;
    } else {
        this->state = false;
    }
}

void InterruptingButton::loop(const unsigned long int &micro) {
    if (this->state) {
        this->_press(micro);
    } else {
        this->_release(micro);
    }

    this->_handleEvents();
}
