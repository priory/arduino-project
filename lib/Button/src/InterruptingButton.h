#ifndef INTERRUPTINGBUTTON_H
#define INTERRUPTINGBUTTON_H

#include "Arduino.h"
#include "AbstractButton.h"

class InterruptingButton : public AbstractButton {
public:
    volatile bool state = false;

    /**
     * Constructor.
     *
     * @param pin: pin used for the button
     * @param userFunc: function to use for the interrupt
     * @param bounce: interval between pressed
     */
    explicit InterruptingButton(
            uint8_t pin,
            void (*userFunc)(),
            unsigned long int bounce = 200000
    );

    /**
     * Destructor.
     */
    ~InterruptingButton();

    /**
     * Funciton that is executed on interrupt.
     */
    void interruptHandle();

    /**
     * @inheritdoc
     */
    void loop(const unsigned long int &micro) override;
};

#endif //INTERRUPTINGBUTTON_H
