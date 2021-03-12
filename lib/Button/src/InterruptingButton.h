#ifndef INTERRUPTINGBUTTON_H
#define INTERRUPTINGBUTTON_H

#ifndef ABSTRACTBUTTON_H

#include "AbstractButton.h"

#endif

#include "Arduino.h"

class InterruptingButton : public AbstractButton {
public:
    explicit InterruptingButton(
            uint8_t pin,
            void (*userFunc)(),
            unsigned long int bounce = 200000
    );

    ~InterruptingButton();

    void interruptHandle();

    void loop(const unsigned long int &micro) override;

private:
    volatile bool _interrupted = false;
};

#endif //INTERRUPTINGBUTTON_H


