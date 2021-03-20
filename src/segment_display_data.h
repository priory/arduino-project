#ifndef SEGMENT_DISPLAY_DATA_H
#define SEGMENT_DISPLAY_DATA_H

const uint8_t SD_PINS[8] PROGMEM = {2, 3, 4, 5, 6, 8, 7, 0};

const uint8_t SD_MAP[19] PROGMEM = {
        //PGFEDCBA
        0b00111111, // 0  0
        0b00000110, // 1  1
        0b01011011, // 2  2
        0b01001111, // 3  3
        0b01100110, // 4  4
        0b01101101, // 5  5
        0b01111101, // 6  6
        0b00000111, // 7  7
        0b01111111, // 8  8
        0b01101111, // 9  9
        0b00000000, // 10 clear
        0b00000001, // 11 seg a
        0b00000010, // 12 seg b
        0b00000100, // 13 seg c
        0b00001000, // 14 seg d
        0b00010000, // 15 seg e
        0b00100000, // 16 seg f
        0b01000000, // 17 seg g
        0b10000000, // 18 seg dp
};

#endif //SEGMENT_DISPLAY_DATA_H
