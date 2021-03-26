#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <Arduino.h>
#include <avr/pgmspace.h>

template<uint16_t S, uint8_t N>
class MusicPlayer {
public:
    /**
     * Constructor.
     *
     * @param pin: pin used to play music
     */
    explicit MusicPlayer(uint8_t pin);

    /**
     * Function to execute when the track has ended.
     *
     * @param onEnd: Pointer to the function
     */
    void setOnEnd(void (*onEnd)());

    /**
     * Reads the frequency of the note from PROGMEM.
     *
     * @param note: Note index
     * @return frequency
     */
    uint16_t readFrequency(uint8_t note);

    /**
     * Reads the note's property of the track from PROGMEM.
     *
     * @param note: Track note index
     * @param prop: 0 = note index, 1 = duration in ticks
     * @return
     */
    uint8_t readNote(unsigned int note, uint8_t prop = 0);

    /**
     * Sets the notes array pointer. Must be PROGMEM.
     *
     * @param notes: Pointer to array PROGMEM
     */
    void setNotes(const uint16_t (*notes)[N]);

    /**
     * Sets the track array pointer. Must be PROGMEM.
     *
     * Each note must be { NOTE_INDEX, DURATION }
     * NOTE_INDEX is the index of _notes array pointer.
     * DURATION is the dudration in ticks. 1/4th note is 48 ticks.
     *
     * @param track: Pointer to array PROGMEM
     */
    void setTrack(const uint8_t (*track)[S][2]);

    /**
     * Returns whether the track is playing or not.
     *
     * @return
     */
    bool isPlaying();

    /**
     * Gets the Beats per Minute.
     *
     * @return
     */
    uint8_t getBPM();

    /**
     * Sets the Beats per Minute.
     *
     * @param bpm
     */
    void setBPM(uint8_t bpm);

    /**
     * Gets the number of notes a track has.
     *
     * @return
     */
    unsigned int getTrackNotes();

    /**
     * Sets the number of notes a track has.
     *
     * @param notes
     */
    void setTrackNotes(unsigned int notes);

    /**
     * Sets the Beats per Minute and number of notes a track has.
     *
     * Same as setBPM and setTrackNotes combined.
     *
     * @param bpm
     * @param notes
     */
    void setupTrack(uint8_t bpm, unsigned int notes);

    /**
     * Resets the interval for ticks.
     *
     * Ticks are used to define the duration of notes.
     * 1/4th note is 48 ticks. A tick has an interval in microseconds.
     * This method calculates the interval based on BPM.
     *
     * Formula:
     * MICRO_SECONDS_IN_MINUTE / TICKS_PER_4_QUARTER_NOTES / BEATS_PER_MINUTE
     * = 60,000,000 / 192 / BPM = 312,500 / BPM
     */
    void resetInterval();

    /**
     * Loop function that is used to run the music player.
     *
     * @param micro:
     */
    void loop(unsigned long int &micro);

    /**
     * Plays the music player.
     */
    void play();

    /**
     * Pauses the music player.
     */
    void pause();

    /**
     * Resets the music player to first note.
     */
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
void MusicPlayer<S, N>::setOnEnd(void (*onEnd)()) {
    _onEnd = onEnd;
}

template<uint16_t S, uint8_t N>
uint16_t MusicPlayer<S, N>::readFrequency(uint8_t note) {
    return pgm_read_word(&_notes[0][note]);
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
