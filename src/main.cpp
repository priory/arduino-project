#include <Arduino.h>
#include "../lib/AnalogIO/src/AnalogInput.h"
#include "../lib/Button/src/DigitalButton.h"
#include "../lib/MusicPlayer/src/MusicPlayer.h"

#include "music_data.h"

AnalogInput *ldr;
AnalogInput *pot;

DigitalButton *playButton;
DigitalButton *nextButton;

MusicPlayer<256, 52> *musicPlayer;

uint8_t redLED = 16;
uint8_t greenLED = 17;

uint8_t track = 4;
bool musicEnabled = false;

void setTrack(uint8_t t) {
    musicPlayer->setTrack(&TRACKS[track]);
    musicPlayer->setupTrack(
            ((uint8_t) pgm_read_byte(&TRACK_PROPS[track][0])) * (pot->value() * 0.75 + 0.25),
            (uint16_t) pgm_read_byte(&TRACK_PROPS[track][1])
    );
    musicPlayer->reset();
}

void nextTrack() {
    track++;
    if (track >= NUM_OF_TRACKS) track = 0;
    setTrack(track);
}

void setup() {
    Serial.begin(9600);

    ldr = new AnalogInput(A1, 1, 100, 10, 10, 100000);
    pot = new AnalogInput(A0, 89, 1018, 5, 5, 1000);

    musicPlayer = new MusicPlayer<256, 52>(11);

    musicPlayer->setNotes(&NOTES);
    setTrack(track);

    playButton = new DigitalButton(12);
    nextButton = new DigitalButton(13);

    musicPlayer->setOnEnd([] { musicPlayer->reset(); });

    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);

    ldr->setOnChange([] {
        if (ldr->value() > 0.03) {
            digitalWrite(greenLED, HIGH);
            digitalWrite(redLED, LOW);
            if (musicEnabled) musicPlayer->play();
        } else {
            digitalWrite(greenLED, LOW);
            digitalWrite(redLED, HIGH);
            musicPlayer->pause();
        }
        Serial.println(ldr->value());
    });

    pot->setOnChange([] {
        musicPlayer->setBPM(((uint8_t) pgm_read_byte(&TRACK_PROPS[track][0])) * (pot->value() * 0.75 + 0.25));
    });

    playButton->setOnPressed([] {
        if (musicEnabled) {
            musicEnabled = false;
            musicPlayer->pause();
        } else {
            musicEnabled = true;
            musicPlayer->play();
        }
    });

    nextButton->setOnPressed([] {
        nextTrack();
    });
}

unsigned long int counter = 0;
unsigned long int interval = 100000;

void loop() {
    unsigned long int micro = micros();

    ldr->loop(micro);
    pot->loop(micro);
    playButton->loop(micro);
    nextButton->loop(micro);
    musicPlayer->loop(micro);

//    if (micro - counter >= interval) {
//
//        counter = micro;
//    }
}
