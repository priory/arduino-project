#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <Arduino.h>
#include <avr/pgmspace.h>

template<uint16_t S, uint8_t N>
class MusicPlayer {
public:
    explicit MusicPlayer(uint8_t pin);

    void setOnEnd(void (*onEnd)()) {
        _onEnd = onEnd;
    }

    uint16_t readFrequency(uint8_t note);

    uint8_t readNote(unsigned int note, uint8_t prop = 0);

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
    void (*_onEnd)() = []{};

    uint8_t nextNote(uint8_t prop = 0);

    void killNote();

    void playNote();
};

template<uint16_t S, uint8_t N>
MusicPlayer<S, N>::MusicPlayer(uint8_t pin) : _pin(pin) {
    pinMode(_pin, OUTPUT);
}

template<uint16_t S, uint8_t N>
uint16_t MusicPlayer<S, N>::readFrequency(uint8_t note) {
    return pgm_read_ptr(&_notes[0][note]);
}


template<uint16_t S, uint8_t N>
uint8_t MusicPlayer<S, N>::readNote(unsigned int note, uint8_t prop) {
    return pgm_read_byte(&_track[0][note][prop]);
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::setNotes(const uint16_t (*notes)[N]) {
    _notes = notes;
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::setTrack(const uint8_t (*track)[S][2]) {
    _track = track;
}

template<uint16_t S, uint8_t N>
bool MusicPlayer<S, N>::isPlaying() {
    return _playing;
}

template<uint16_t S, uint8_t N>
uint8_t MusicPlayer<S, N>::getBPM() {
    return _trackBPM;
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::setBPM(uint8_t bpm) {
    _trackBPM = bpm;
    resetInterval();
}

template<uint16_t S, uint8_t N>
unsigned int MusicPlayer<S, N>::getTrackNotes() {
    return _trackNotes;
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::setTrackNotes(unsigned int notes) {
    _trackNotes = notes;
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::setupTrack(uint8_t bpm, unsigned int notes) {
    setBPM(bpm);
    setTrackNotes(notes);
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::resetInterval() {
    _interval = 312500 / getBPM();
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::loop(unsigned long &micro) {
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
                    _onEnd();
                }
            }
            _micro = micro;
        }
    }
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::play() {
    _playing = true;
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::pause() {
    killNote();
    _playing = false;
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::reset() {
    resetInterval();
    _micro = 0;
    _note = 0;
    _tick = 0;
}

template<uint16_t S, uint8_t N>
uint8_t MusicPlayer<S, N>::nextNote(uint8_t prop) {
    return readNote(_note, prop);
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::killNote() {
    noTone(_pin);
}

template<uint16_t S, uint8_t N>
void MusicPlayer<S, N>::playNote() {
    unsigned int n = this->readFrequency(nextNote());
    if (n) {
        tone(_pin, n);
    } else {
        killNote();
    }
}

#endif //MUSICPLAYER_H
