#include <Arduino.h>
#include "../lib/Button/src/DigitalButton.h"

unsigned int presses = 0;

void onPressed() {
    Serial.println("HIGH");
    presses++;
}

void onReleased() {
    Serial.println("LOW");
    Serial.println(presses);
}

DigitalButton *btn;

void setup() {
    Serial.begin(9600);

    btn = new DigitalButton(13);

    btn->setOnPressed(onPressed);
    btn->setOnReleased(onReleased);
}

void loop() {
    unsigned long int micro = micros();

    btn->loop(micro);
}
