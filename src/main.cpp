#include <Arduino.h>

class InterruptingButton {
public:
    InterruptingButton(unsigned char pin, void (*userFunc)(), unsigned long int bounce = 200000) : _pin(pin),
                                                                                                   _bounce(bounce) {
        attachInterrupt(digitalPinToInterrupt(this->_pin), userFunc, CHANGE);
    }

    ~InterruptingButton() {
        detachInterrupt(digitalPinToInterrupt(this->_pin));
    };

    void interruptHandle() {
        if (digitalRead(this->_pin)) {
            this->_high = true;
        } else {
            this->_high = false;
        }
    }

    bool isHigh() const {
        return this->_high;
    }

    void loop(unsigned long int micro) {
        if (this->_high) {
            if (!this->_changed) {
                if (micro - this->_lastPress >= this->_bounce) {
                    this->_pressed = true;
                    this->_released = false;
                    this->_lastRelease = 0;
                    this->_lastPress = micro;
                    this->_changed = true;
                }
            }
        } else {
            if (this->_changed) {
                if (micro - this->_lastRelease >= this->_bounce) {
                    this->_pressed = false;
                    this->_released = true;
                    this->_lastRelease = micro;
                    this->_changed = false;
                }
            }
        }

        if (this->_pressed) {
            this->_pressed = false;
            this->_onPressed();
        }

        if (this->_released) {
            this->_released = false;
            this->_onReleased();
        }
    }

    void setOnPressed(void (*func)()) {
        this->_onPressed = func;
    }

    void setOnReleased(void (*func)()) {
        this->_onReleased = func;
    }

private:
    unsigned char _pin;
    volatile bool _high = false;
    bool _changed = false;
    unsigned long int _lastPress = 0;
    unsigned long int _lastRelease = 0;
    unsigned long int _bounce;
    bool _pressed = false;
    bool _released = false;

    void (*_onPressed)() = [] {};

    void (*_onReleased)() = [] {};
};

unsigned int presses = 0;

void onPressed() {
    Serial.println("HIGH");
    presses++;
}

void onReleased() {
    Serial.println("LOW");
    Serial.println(presses);

}

InterruptingButton *btn;

void setup() {
    Serial.begin(9600);
    Serial.println("BEGIN");

    btn = new InterruptingButton(2, [] { btn->interruptHandle(); });

    btn->setOnPressed(onPressed);
    btn->setOnReleased(onReleased);
}

unsigned long int counter = 0;
unsigned int interval = 1000;

void loop() {
    unsigned long int micro = micros();

    btn->loop(micro);
}
