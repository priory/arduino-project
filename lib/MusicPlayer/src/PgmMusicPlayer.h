//
// Created by vpack on 2021-03-14.
//
#include <Arduino.h>

#ifndef PGMMUSICPLAYER_H
#define PGMMUSICPLAYER_H

template<uint16_t S, uint8_t N>
class PgmMusicPlayer {
public:
    explicit PgmMusicPlayer(uint8_t pin);

    uint16_t readFrequency(uint8_t note);

    void setNotes(const uint16_t (*notes)[N]);

    void setTrack(const uint8_t (*track)[S][2]);

    bool isPlaying();

    uint8_t getBPM();

    void setBPM(uint8_t bpm);

    unsigned int getTrackNotes();

    void setTrackNotes(unsigned int notes);

    void setupTrack(uint8_t bpm, unsigned int notes);

    void resetInterval();

    void loop(unsigned long int &micro);

    void play();

    void pause();

    void reset();

private:
    uint8_t _pin;
    bool _playing = false;
    unsigned long int _micro = 0;
    unsigned int _tick = 0;
    unsigned int _interval = 0;
    unsigned int _note = 0;
    const uint16_t (*_notes)[N];
    const uint8_t (*_track)[S][2];
    uint8_t _trackBPM = 120;
    unsigned int _trackNotes = 0;

    uint8_t getNoteProp(unsigned int note, uint8_t prop = 0);

    uint8_t nextNote(uint8_t prop = 0);

    void killNote();

    void playNote();
};

template<uint16_t S, uint8_t N>
PgmMusicPlayer<S, N>::PgmMusicPlayer(uint8_t pin) : _pin(pin) {
    pinMode(_pin, OUTPUT);
}

template<uint16_t S, uint8_t N>
uint16_t PgmMusicPlayer<S, N>::readFrequency(uint8_t note) {
    return (uint16_t) pgm_read_ptr(&_notes[0][note]);
}

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::setNotes(const uint16_t (*notes)[N]) {
    _notes = notes;
}

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::setTrack(const uint8_t (*track)[S][2]) {
    _track = track;
}

template<uint16_t S, uint8_t N>
bool PgmMusicPlayer<S, N>::isPlaying() {
    return _playing;
}

template<uint16_t S, uint8_t N>
uint8_t PgmMusicPlayer<S, N>::getBPM() {
    return _trackBPM;
}

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::setBPM(uint8_t bpm) {
    _trackBPM = bpm;
    resetInterval();
}

template<uint16_t S, uint8_t N>
unsigned int PgmMusicPlayer<S, N>::getTrackNotes() {
    return _trackNotes;
}

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::setTrackNotes(unsigned int notes) {
    _trackNotes = notes;
}

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::setupTrack(uint8_t bpm, unsigned int notes) {
    setBPM(bpm);
    setTrackNotes(notes);
}

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::resetInterval() {
    _interval = 312500 / getBPM();
}

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::loop(unsigned long &micro) {
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

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::play() {
    _playing = true;
}

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::pause() {
    killNote();
    _playing = false;
}

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::reset() {
    resetInterval();
    _micro = 0;
    _note = 0;
    _tick = 0;
}

template<uint16_t S, uint8_t N>
uint8_t PgmMusicPlayer<S, N>::getNoteProp(unsigned int note, uint8_t prop) {
    return pgm_read_byte(&_track[0][note][prop]);
}

template<uint16_t S, uint8_t N>
uint8_t PgmMusicPlayer<S, N>::nextNote(uint8_t prop) {
    return getNoteProp(_note, prop);
}

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::killNote() {
    noTone(_pin);
}

template<uint16_t S, uint8_t N>
void PgmMusicPlayer<S, N>::playNote() {
    unsigned int n = this->readFrequency(nextNote());
    if (n) {
        tone(_pin, n);
    } else {
        killNote();
    }
}

#endif //PGMMUSICPLAYER_H
