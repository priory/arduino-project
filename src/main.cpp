#include <Arduino.h>
#include "../lib/Button/src/InterruptingButton.h"
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

//DigitalButton *btn;
InterruptingButton *btn;

void setup() {
    Serial.begin(9600);

//    btn = new DigitalButton(13);

//    btn->setOnPressed(onPressed);
//    btn->setOnReleased(onReleased);

    btn = new InterruptingButton(2, [] { btn->interruptHandle(); });

    btn->setOnPressed(onPressed);
    btn->setOnReleased(onReleased);
}

unsigned long int counter = 0;
unsigned int interval = 1000;

void loop() {
    unsigned long int micro = micros();

    btn->loop(micro);
//    if (micro - counter >= interval) {
//
//        counter = micro;
//    }
//    btn->loop(micro);
}
