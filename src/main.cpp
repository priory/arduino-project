#include <Arduino.h>
#include "../lib/AnalogIO/src/AnalogInput.h"
#include "../lib/Button/src/DigitalButton.h"
#include "../lib/MusicPlayer/src/MusicPlayer.h"
#include "../lib/SegmentDisplay/src/SegmentDisplay.h"

#include "music_data.h"
#include "segment_display_data.h"

/******************************************************************************
 * MODULES
 ******************************************************************************/

// Light Dependent Resistor
AnalogInput *ldr;

// Potentiometer
AnalogInput *pot;

// Play button
DigitalButton *playButton;

// Next button
DigitalButton *nextButton;

// Music player
MusicPlayer<256, 52> *musicPlayer;

// 7-segment display
SegmentDisplay *segmentDisplay;

/******************************************************************************
 * VARIABLES
 ******************************************************************************/

/**
 * Constants
 */

// LDR threshold
const float ldrSensitivity = 0.4;

// Red LED pin
const uint8_t redLED = 9;

// Green LED pin
const uint8_t greenLED = 10;

// Music player pin
const uint8_t musicPlayerPin = 11;

/**
 * Variables
 */

// Current track index
uint8_t track = 0;

// Music enablement
bool musicEnabled = true;

/******************************************************************************
 * FUNCTIONS
 ******************************************************************************/

/**
 * Sets the track for the music player and resets it.
 *
 * @param t: track
 */
void setTrack(uint8_t t) {
    musicPlayer->setTrack(&TRACKS[track]);
    musicPlayer->setupTrack(
            ((uint8_t) pgm_read_byte(&TRACK_PROPS[track][0])) * (pot->value() * 0.75 + 0.25),
            (uint16_t) pgm_read_byte(&TRACK_PROPS[track][1])
    );
    musicPlayer->reset();
}

/**
 * Sets the next track.
 */
void nextTrack() {
    track++;
    if (track >= NUM_OF_TRACKS) track = 0;
    setTrack(track);
    segmentDisplay->render(track + 1);
}

/******************************************************************************
 * EVENT HANDLERS
 ******************************************************************************/

/**
 * Plays/pauses the music player depending on the LDR.
 *
 * Plays when it's light and turns on the green LED.
 * Pauses when it's dark and turns on the red LED.
 */
void ldrChangeHandler() {
    if (ldr->value() > ldrSensitivity) {
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, LOW);
        if (musicEnabled && !musicPlayer->isPlaying()) musicPlayer->play();
    } else {
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        if (musicPlayer->isPlaying()) musicPlayer->pause();
    }
}

/**
 * Changes the BPM of the music player depending on the potentiometer.
 *
 * Plays slower if turned up to 25% speed.
 */
void potChangedHandler() {
    musicPlayer->setBPM(((uint8_t) pgm_read_byte(&TRACK_PROPS[track][0])) * (pot->value() * 0.75 + 0.25));
}

/**
 * Enable/disable the music player when the play button is pressed.
 */
void playButtonPressedHandler() {
    if (musicPlayer->isPlaying()) {
        musicEnabled = false;
        musicPlayer->pause();
    } else {
        musicEnabled = true;
        musicPlayer->play();
    }
}

/**
 * Queue the next track when the next button is pressed.
 */
void nextButtonPressedHandler() {
    nextTrack();
}

/**
 * Setup.
 */
void setup() {
    // Setup colored LED's
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);

    // Setup LDR
    ldr = new AnalogInput(A1, 1, 100, 3, 20, 50000);
    ldr->setOnChange(ldrChangeHandler);

    // Setup potentiometer
    pot = new AnalogInput(A0, 89, 1018, 5, 5, 10000);
    pot->setOnChange(potChangedHandler);

    // Setup music player
    musicPlayer = new MusicPlayer<256, 52>(musicPlayerPin);
    musicPlayer->setOnEnd([] { musicPlayer->reset(); });
    musicPlayer->setNotes(&NOTES);
    setTrack(track);

    // Setup buttons
    playButton = new DigitalButton(12);
    playButton->setOnPressed(playButtonPressedHandler);

    nextButton = new DigitalButton(13);
    nextButton->setOnPressed(nextButtonPressedHandler);

    // Setup segment display
    segmentDisplay = new SegmentDisplay(&SD_PINS, &SD_MAP);
    segmentDisplay->render(track + 1);

    // Invoke analog modules for initial setup
    ldr->triggerOnChange();
    pot->triggerOnChange();
}

/**
 * Loop.
 */

unsigned long int counter = 0;
unsigned long int interval = 10000;

void loop() {
    unsigned long int micro = micros();

    if (micro - counter >= interval) {
        ldr->loop(micro);
        pot->loop(micro);
        playButton->loop(micro);
        nextButton->loop(micro);

        counter = micro;
    }

    musicPlayer->loop(micro);
}
