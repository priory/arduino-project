#include <Arduino.h>
#include "AbstractButton.h"

AbstractButton::AbstractButton(
        uint8_t pin,
        unsigned long bounce
) : _pin(pin), _bounce(bounce) {}

uint8_t AbstractButton::getPin() const {
    return _pin;
}

bool AbstractButton::isHigh() const {
    return _high;
}

void AbstractButton::setOnPressed(void (*func)()) {
    _onPressed = func;
}

void AbstractButton::setOnReleased(void (*func)()) {
    _onReleased = func;
}

void AbstractButton::_press(const unsigned long int &micro) {
    if (!isHigh()) {
        if (micro - _lastPress >= _bounce) {
            _pressed = true;
            _released = false;
            _lastRelease = 0;
            _lastPress = micro;
            _high = true;
        }
    }
}

void AbstractButton::_release(const unsigned long int &micro) {
    if (isHigh()) {
        if (micro - _lastRelease >= _bounce) {
            _pressed = false;
            _released = true;
            _lastRelease = micro;
            _lastPress = micro;
            _high = false;
        }
    }
}

void AbstractButton::_handleEvents() {
    if (_pressed) {
        _pressed = false;
        _onPressed();
    }

    if (_released) {
        _released = false;
        _onReleased();
    }
}
