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
    if (digitalRead(this->_pin)) {
        this->_interrupted = true;
    } else {
        this->_interrupted = false;
    }
}

void InterruptingButton::loop(const unsigned long int &micro) {
    if (this->_interrupted) {
        this->_press(micro);
    } else {
        this->_release(micro);
    }

    this->_handleEvents();
}
