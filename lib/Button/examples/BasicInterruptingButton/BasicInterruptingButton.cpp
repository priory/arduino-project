#include <Arduino.h>
#include "../lib/Button/src/InterruptingButton.h"

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

    btn = new InterruptingButton(2, [] { btn->interruptHandle(); });

    btn->setOnPressed(onPressed);
    btn->setOnReleased(onReleased);
}

void loop() {
    unsigned long int micro = micros();

    btn->loop(micro);
}
