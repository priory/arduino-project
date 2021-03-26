#ifndef ABSTRACTBUTTON_H
#define ABSTRACTBUTTON_H

class AbstractButton {
public:
    /**
     * Constructor.
     *
     * Bouncing is needed to ignore unexpected current.
     *
     * @param pin: pin used to read button
     * @param bounce: interval between presses
     */
    explicit AbstractButton(uint8_t pin, unsigned long int bounce = 200000);

    /**
     * Gets the pin used for the button.
     *
     * @return
     */
    uint8_t getPin() const;

    /**
     * Checks if the button is pressed.
     *
     * @return
     */
    bool isHigh() const;

    /**
     * Loop function used to check when an event should be invoked.
     *
     * @param micro
     */
    virtual void loop(const unsigned long int &micro) = 0;

    /**
     * Sets on pressed event handler function.
     *
     * @param func
     */
    void setOnPressed(void (*func)());

    /**
     * Sets on released event handler function.
     *
     * @param func
     */
    void setOnReleased(void (*func)());

protected:
    uint8_t _pin;
    bool _high = false;
    unsigned long int _lastPress = 0;
    unsigned long int _lastRelease = 0;
    unsigned long int _bounce;
    bool _pressed = false;
    bool _released = false;

    void (*_onPressed)() = [] {};

    void (*_onReleased)() = [] {};

    void _press(const unsigned long int &micro);

    void _release(const unsigned long int &micro);

    void _handleEvents();
};

#endif //ABSTRACTBUTTON_H
