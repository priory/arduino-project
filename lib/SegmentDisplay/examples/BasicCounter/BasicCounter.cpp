#include <Arduino.h>
#include "../lib/SegmentDisplay/src/SegmentDisplay.h"
#include "../lib/SegmentDisplay/src/segment_display_data.h"

SegmentDisplay *sd;

void setup() {
    sd = new SegmentDisplay(&SD_PINS, &SD_MAP);
    sd->render(0);
}

unsigned long int counter = 0;
unsigned long int interval = 1000000;
uint8_t digit = 0;

void loop() {
    unsigned long int micro = micros();

    if (micro - counter >= interval) {
        digit++;
        if (digit > 9) digit = 0;

        sd->render(digit);

        sd->point(digit % 2);

        counter = micro;
    }
}
