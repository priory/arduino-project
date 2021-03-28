#include <Arduino.h>
#include "../lib/AnalogIO/src/AnalogInput.h"

AnalogInput *ldr;

void setup() {
    Serial.begin(9600);

    // LDR 1k ohm
    ldr = new AnalogInput(A0, 22, 338, 10, 10);

    ldr->setOnChange([] {
        Serial.println(ldr->value());
    });
}

unsigned long int counter = 0;
unsigned long int interval = 100000;

void loop() {
    unsigned long int micro = micros();

    if (micro - counter >= interval) {
        ldr->loop(micro);

        counter = micro;
    }
}
