#include <Arduino.h>
#include <avr/pgmspace.h>
#include "../lib/Button/src/DigitalButton.h"
#include "music_data.h"

template<uint16_t MAX_NOTES_IN_TRACK, uint8_t NUM_OF_NOTES>
class MusicPlayer {
public:
    explicit MusicPlayer(uint8_t pin) : _pin(pin) {
        pinMode(_pin, OUTPUT);
    }

    void setNotes(const uint16_t (*notes)[NUM_OF_NOTES]) {
        _notes = notes;
    }

    void setTrack(const uint8_t (*track)[MAX_NOTES_IN_TRACK][2]) {
        _track = track;
    }

    bool isPlaying() {
        return _playing;
    }

    uint8_t getBPM() {
        return _trackBPM;
    }

    void setBPM(uint8_t bpm) {
        _trackBPM = bpm;
        resetInterval();
    }

    unsigned int getTrackNotes() {
        return _trackNotes;
    }

    void setTrackNotes(unsigned int notes) {
        _trackNotes = notes;
    }

    void setupTrack(uint8_t bpm, unsigned int notes) {
        setBPM(bpm);
        setTrackNotes(notes);
    }

    void resetInterval() {
        _interval = 312500 / getBPM();
    }

    void loop(unsigned long int &micro) {
        if (isPlaying()) {
            if (micro - _micro >= _interval) {
                if (_tick == 0) {
                    playNote();
                }
                _tick++;
                if (_tick >= nextNote(1) * 4) {
                    killNote();
                    _tick = 0;
                    _note++;
                    if (_note >= getTrackNotes()) {
                        reset();
                    }
                }
                _micro = micro;
            }
        }
    }

    void play() {
        _playing = true;
    }

    void pause() {
        killNote();
        _playing = false;
    }

    void reset() {
        resetInterval();
        _micro = 0;
        _note = 0;
        _tick = 0;
    }

private:
    uint8_t _pin;
    bool _playing = false;
    unsigned long int _micro = 0;
    unsigned int _tick = 0;
    unsigned int _interval = 0;
    unsigned int _note = 0;
    const uint16_t (*_notes)[NUM_OF_NOTES];
    const uint8_t (*_track)[MAX_NOTES_IN_TRACK][2];
    uint8_t _trackBPM = 120;
    unsigned int _trackNotes = 0;

    uint8_t getNoteProp(unsigned int note, uint8_t prop = 0) {
        return pgm_read_byte(&_track[0][note][prop]);
    }

    uint8_t nextNote(uint8_t prop = 0) {
        return getNoteProp(_note, prop);
    }

    void killNote() {
        noTone(_pin);
    }

    void playNote() {
        unsigned int n = (uint16_t)pgm_read_ptr(&_notes[0][nextNote()]);
        if (n) {
            tone(_pin, n);
        } else {
            killNote();
        }
    }
};

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
unsigned long int interval = 100;

void loop() {
    unsigned long int micro = micros();

    if (micro - counter >= interval) {

        counter = micro;
    }

    mp->loop(micro);
    btnA->loop(micro);
    btnB->loop(micro);
}
