#include <Arduino.h>
#include <avr/pgmspace.h>
#include "../lib/MusicPlayer/src/MusicPlayer.h"

MusicPlayer<64, 5> *mp;

/**
 * Bare minimum notes for a simple track
 *
 * A4 = 440 Hz
 * @see https://www.szynalski.com/tone-generator/
 */
const uint16_t NOTES[5] PROGMEM = {
        0,   //Rest 0
        494, //B4   1
        622, //D#5  2
        659, //E5   3
        740, //F#5  4
};

/**
 * {NOTE, TICKS}
 * There are 192 ticks in one bar
 * (96 ticks in one second with 120 bpm)
 *
 * Street Fighter - Guile's Theme
 * @see https://www.songsterr.com/a/wsa/super-street-fighter-ii-guiles-theme-power-trio-tab-s457584t3
 */
//@formatter:off
const uint8_t TRACK[64][2] PROGMEM = {
        {3, 12}, {0, 12}, {3, 12}, {2, 12},  {0, 12}, {2, 12},  {3, 96},  {2, 12}, {0, 12},
        {3, 12}, {0, 12}, {3, 12}, {2, 12},  {0, 12}, {2, 12},  {3, 96},  {2, 12}, {0, 12},
        {3, 12}, {2, 12}, {0, 12}, {3, 12},  {0, 12}, {2, 12}, {0, 12}, {4, 12},  {0, 12}, {4, 12}, {3, 24},  {2, 24}, {1, 24}
};
//@formatter:on

// Example storing and reading data from flash memory.
const uint8_t TRACK_PROPS[2] PROGMEM = {120, 31};

void setup() {
    Serial.begin(9600);

    mp = new MusicPlayer<64, 5>(11);

    mp->setNotes(&NOTES);
    mp->setTrack(&TRACK);
    mp->setupTrack(
            (uint8_t) pgm_read_byte(&TRACK_PROPS[0]),
            (uint16_t) pgm_read_byte(&TRACK_PROPS[1])
    );

    // End of track basic loop example
    // Can be used to implement own logic
    // (next track for example etc.)
    mp->setOnEnd([] { mp->reset(); });

    mp->play();
}

void loop() {
    unsigned long int micro = micros();

    mp->loop(micro);
}
