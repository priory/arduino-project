#include <Arduino.h>
#include "../lib/AnalogIO/src/AnalogInput.h"

AnalogInput *ldr;
AnalogInput *pot;

void setup() {
    Serial.begin(9600);

    ldr = new AnalogInput(A1, 1, 100, 10, 10, 100000);
    pot = new AnalogInput(A0, 89, 1018, 5, 5, 1000);

    ldr->setOnChange([] {
        Serial.println(ldr->value());
    });

    pot->setOnChange([] {
        Serial.println(pot->value());
    });
}

unsigned long int counter = 0;
unsigned long int interval = 100000;

void loop() {
    unsigned long int micro = micros();

    ldr->loop(micro);
    pot->loop(micro);

//    if (micro - counter >= interval) {
//
//        counter = micro;
//    }
}

/*#include <Arduino.h>
#include "../lib/AnalogIO/src/AnalogInput.h"

AnalogInput *pot;

void setup() {
    Serial.begin(9600);

    pinMode(2, OUTPUT);

    // LDR 1k ohm
    pot = new AnalogInput(A0, 22, 338, 10, 10);

    pot->setOnChange([] {
        if (pot->value() > 0.5) {
            digitalWrite(2, HIGH);
        } else {
            digitalWrite(2, LOW);
        }
    });
}

unsigned long int counter = 0;
unsigned long int interval = 100000;

void loop() {
    unsigned long int micro = micros();

    if (micro - counter >= interval) {
        pot->loop();

        counter = micro;
    }
}*/
