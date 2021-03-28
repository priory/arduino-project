#include "AnalogInput.h"

AnalogInput::AnalogInput(
        const uint8_t pin,
        unsigned int min,
        unsigned int max,
        uint8_t threshold,
        uint8_t stability,
        unsigned long int pollingRate
) : _pin(pin),
    _min(min),
    _max(max),
    _threshold(threshold),
    _stability(stability),
    _pollingRate(pollingRate) {
    _previous = analogRead(_pin);
}

void AnalogInput::setOnChange(void (*onChange)()) {
    _onChange = onChange;
}

void AnalogInput::triggerOnChange() {
    _onChange();
}

void AnalogInput::loop(unsigned long int &micro) {
    unsigned int read = analogRead(_pin);

    if (micro - _counter >= _pollingRate) {
        if ((read > (_previous + _threshold)) || ((signed int) read < ((signed int) _previous - _threshold))) {
            _setOffs++;

            if (_setOffs >= _stability) {
                _previous = read;
                _setOffs = 0;
                triggerOnChange();
            };
        } else {
            _setOffs = 0;
        }

        _counter = micro;
    }
}

float AnalogInput::value() const {
    return ((float) (max(min(_previous, _max), _min) - _min) / (float) (_max - _min));
}

unsigned int AnalogInput::read() const {
    return _previous;
}
