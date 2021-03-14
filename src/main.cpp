#include <Arduino.h>
#include "../lib/Button/src/DigitalButton.h"
#include "../lib/MusicPlayer/src/MusicPlayer.h"
#include "music_data.h"

MusicPlayer<256, 52> *mp;

DigitalButton *btnA;
DigitalButton *btnB;

bool toggleBPM = false;

void setup() {
    Serial.begin(9600);

    mp = new MusicPlayer<256, 52>(11);

    mp->setNotes(&NOTES);
    mp->setTrack(&ALL_TRACKS[0]);
    mp->setupTrack(146, 108);
    mp->setOnEnd([]{
        mp->reset();
        mp->setTrack(&ALL_TRACKS[1]);
        mp->setupTrack(200, 64);
    });

    btnA = new DigitalButton(12);
    btnA->setOnPressed([] {
        if (mp->isPlaying()) mp->pause();
        else mp->play();
    });

    btnB = new DigitalButton(13);
    btnB->setOnPressed([] {
        toggleBPM ^= 1;
        if (toggleBPM) mp->setBPM(73);
        else mp->setBPM(146);
    });
}

unsigned long int counter = 0;
unsigned long int interval = 1000;

void loop() {
    unsigned long int micro = micros();

    if (micro - counter >= interval) {

        counter = micro;
    }

    mp->loop(micro);
    btnA->loop(micro);
    btnB->loop(micro);
}
