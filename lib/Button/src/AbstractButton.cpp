#include <Arduino.h>

#ifndef ABSTRACTBUTTON_H

#include "AbstractButton.h"

#endif

AbstractButton::AbstractButton(
        uint8_t pin,
        unsigned long bounce
) : _pin(pin), _bounce(bounce) {}

bool AbstractButton::isHigh() const {
    return this->_high;
}

void AbstractButton::setOnPressed(void (*func)()) {
    this->_onPressed = func;
}

void AbstractButton::setOnReleased(void (*func)()) {
    this->_onReleased = func;
}

void AbstractButton::_press(const unsigned long int &micro) {
    if (!this->isHigh()) {
        if (micro - this->_lastPress >= this->_bounce) {
            this->_pressed = true;
            this->_released = false;
            this->_lastRelease = 0;
            this->_lastPress = micro;
            this->_high = true;
        }
    }
}

void AbstractButton::_release(const unsigned long int &micro) {
    if (this->isHigh()) {
        if (micro - this->_lastRelease >= this->_bounce) {
            this->_pressed = false;
            this->_released = true;
            this->_lastRelease = micro;
            this->_lastPress = micro;
            this->_high = false;
        }
    }
}

void AbstractButton::_handleEvents() {
    if (this->_pressed) {
        this->_pressed = false;
        this->_onPressed();
    }

    if (this->_released) {
        this->_released = false;
        this->_onReleased();
    }
}
