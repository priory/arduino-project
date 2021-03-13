#ifndef DIGITALBUTTON_H
#define DIGITALBUTTON_H

#ifndef ABSTRACTBUTTON_H

#include "AbstractButton.h"

#endif

class DigitalButton : public AbstractButton {
public:
    explicit DigitalButton(
            uint8_t pin,
            unsigned long int bounce = 200000,
            uint8_t mode = INPUT);

    void loop(const unsigned long int &micro) override;
};

#endif //DIGITALBUTTON_H