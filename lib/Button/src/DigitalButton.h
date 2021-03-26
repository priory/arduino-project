#ifndef DIGITALBUTTON_H
#define DIGITALBUTTON_H

#include "AbstractButton.h"

class DigitalButton : public AbstractButton {
public:
    /**
     * Constructor.
     *
     * @param pin: pin used for the button
     * @param bounce: interval between pressed
     * @param mode: pin mode for pinMode function
     */
    explicit DigitalButton(
            uint8_t pin,
            unsigned long int bounce = 200000,
            uint8_t mode = INPUT);

    /**
     * @inheritdoc
     */
    void loop(const unsigned long int &micro) override;
};

#endif //DIGITALBUTTON_H
