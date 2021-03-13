#include <Arduino.h>

#ifndef INTERRUPTINGBUTTON_H

#include "InterruptingButton.h"

#endif

InterruptingButton::InterruptingButton(
        uint8_t pin,
        void (*userFunc)(),
        unsigned long int bounce
) : AbstractButton(pin, bounce) {
    attachInterrupt(digitalPinToInterrupt(_pin), userFunc, CHANGE);
}

InterruptingButton::~InterruptingButton() {
    detachInterrupt(digitalPinToInterrupt(_pin));
};

void InterruptingButton::interruptHandle() {
    if (digitalRead(getPin())) {
        state = true;
    } else {
        state = false;
    }
}

void InterruptingButton::loop(const unsigned long int &micro) {
    if (state) {
        _press(micro);
    } else {
        _release(micro);
    }

    _handleEvents();
}
