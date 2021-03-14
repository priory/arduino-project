#ifndef INTERRUPTINGBUTTON_H
#define INTERRUPTINGBUTTON_H

#include "Arduino.h"
#include "AbstractButton.h"

class InterruptingButton : public AbstractButton {
public:
    volatile bool state = false;

    explicit InterruptingButton(
            uint8_t pin,
            void (*userFunc)(),
            unsigned long int bounce = 200000
    );

    ~InterruptingButton();

    void interruptHandle();

    void loop(const unsigned long int &micro) override;
};

#endif //INTERRUPTINGBUTTON_H


