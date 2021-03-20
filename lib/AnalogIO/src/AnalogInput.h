#ifndef ANALOGINPUT_H
#define ANALOGINPUT_H

#include <Arduino.h>

class AnalogInput {
public:
    explicit AnalogInput(
            uint8_t pin,
            unsigned int min = 26,
            unsigned int max = 1022,
            uint8_t threshold = 5,
            uint8_t stability = 1,
            unsigned long int poll = 1000
    );

    void setOnChange(void (*onChange)());

    void triggerOnChange();

    void loop(unsigned long int &micro);

    unsigned int read() const;

    float value() const;

private:
    const uint8_t _pin;
    const unsigned int _min;
    const unsigned int _max;
    const uint8_t _threshold;
    const uint8_t _stability;
    const unsigned long int _poll;
    unsigned long int _counter = 0;
    uint8_t _setOffs = 0;
    unsigned int _previous;

    void (*_onChange)() = [] {};
};

#endif //ANALOGINPUT_H
