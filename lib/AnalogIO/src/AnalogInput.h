#ifndef ANALOGINPUT_H
#define ANALOGINPUT_H

#include <Arduino.h>

class AnalogInput {
public:
    /**
     * Constructor.
     *
     * @param pin: pin used for the analog input
     * @param min: minimum value of the input
     * @param max: maximum value of the input.
     * @param threshold: limit to when input change can trigger onChange
     * @param stability: how many consecutive times threshold must be exceeded
     * @param poll: polling time interval
     */
    explicit AnalogInput(
            uint8_t pin,
            unsigned int min = 26,
            unsigned int max = 1022,
            uint8_t threshold = 5,
            uint8_t stability = 1,
            unsigned long int poll = 1000
    );

    /**
     * Sets on input change event handler function.
     *
     * @param onChange
     */
    void setOnChange(void (*onChange)());

    /**
     * Invokes on input change event.
     */
    void triggerOnChange();

    /**
     * The loop function used to check for analog input.
     *
     * @param micro
     */
    void loop(unsigned long int &micro);

    /**
     * Reads the literal value from the analog sensor.
     *
     * @return
     */
    unsigned int read() const;

    /**
     * Reads the decimal value between 0 and 1.
     *
     * @return
     */
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
