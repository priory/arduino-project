#include <Arduino.h>
#include "../lib/AnalogIO/src/AnalogInput.h"
#include "../lib/Button/src/DigitalButton.h"
#include "../lib/MusicPlayer/src/MusicPlayer.h"
#include "../lib/SegmentDisplay/src/SegmentDisplay.h"

#include "music_data.h"
#include "segment_display_data.h"

AnalogInput *ldr;
AnalogInput *pot;
DigitalButton *playButton;
DigitalButton *nextButton;
MusicPlayer<256, 52> *musicPlayer;
SegmentDisplay *segmentDisplay;

const float ldrSensitivity = 0.4;
const uint8_t redLED = 9;
const uint8_t greenLED = 10;

uint8_t track = 0;
bool musicEnabled = true;

void setTrack(uint8_t t) {
    musicPlayer->setTrack(&TRACKS[track]);
    musicPlayer->setupTrack(
            ((uint8_t) pgm_read_byte(&TRACK_PROPS[track][0])),
            (uint16_t) pgm_read_byte(&TRACK_PROPS[track][1])
    );
    musicPlayer->reset();
}

void nextTrack() {
    track++;
    if (track >= NUM_OF_TRACKS) track = 0;
    setTrack(track);
    segmentDisplay->render(track + 1);
}

/**
 * Event handlers
 */

void ldrChangeHandler() {
    if (ldr->value() > ldrSensitivity) {
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, LOW);
        if (musicEnabled) musicPlayer->play();
    } else {
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        musicPlayer->pause();
    }
}

void potChangedHandler() {
    musicPlayer->setBPM(((uint8_t) pgm_read_byte(&TRACK_PROPS[track][0])) * (pot->value() * 0.75 + 0.25));
}

void playButtonPressedHandler() {
    if (musicEnabled) {
        musicEnabled = false;
        musicPlayer->pause();
    } else {
        musicEnabled = true;
        musicPlayer->play();
    }
}

void nextButtonPressedHandler() {
    nextTrack();
}

void setup() {
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);

    ldr = new AnalogInput(A1, 1, 100, 3, 20, 50000);
    ldr->setOnChange(ldrChangeHandler);

    pot = new AnalogInput(A0, 89, 1018, 5, 5, 1000);
    pot->setOnChange(potChangedHandler);

    musicPlayer = new MusicPlayer<256, 52>(11);
    musicPlayer->setOnEnd([] { musicPlayer->reset(); });
    musicPlayer->setNotes(&NOTES);
    setTrack(track);

    playButton = new DigitalButton(12);
    playButton->setOnPressed(playButtonPressedHandler);

    nextButton = new DigitalButton(13);
    nextButton->setOnPressed(nextButtonPressedHandler);

    segmentDisplay = new SegmentDisplay(&SD_PINS, &SD_MAP);
    segmentDisplay->render(track + 1);

    ldr->triggerOnChange();
    pot->triggerOnChange();
}

void loop() {
    unsigned long int micro = micros();

    ldr->loop(micro);
    pot->loop(micro);
    playButton->loop(micro);
    nextButton->loop(micro);
    musicPlayer->loop(micro);
}
