#ifndef ABSTRACTBUTTON_H
#define ABSTRACTBUTTON_H

class AbstractButton {
public:
    explicit AbstractButton(uint8_t pin, unsigned long int bounce = 200000);

    bool isHigh() const;

    virtual void loop(const unsigned long int &micro) = 0;

    void setOnPressed(void (*func)());

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
