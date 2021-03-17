#include <Arduino.h>
#include <avr/pgmspace.h>

const uint8_t PINS[8] PROGMEM = {2, 3, 4, 5, 6, 8, 7, 9};

const uint8_t MAP[19] PROGMEM = {
        //PGFEDCBA
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111, // 9
        0b00000000, // nothing
        0b00000001, // seg a
        0b00000010, // seg b
        0b00000100, // seg c
        0b00001000, // seg d
        0b00010000, // seg e
        0b00100000, // seg f
        0b01000000, // seg g
        0b10000000, // seg dp
};

class SegmentDisplay {
public:
//    struct RENDER {
//        uint8_t ZERO = 0;
//        uint8_t ONE = 1;
//        uint8_t TWO = 2;
//        uint8_t THREE = 3;
//        uint8_t FOUR = 4;
//        uint8_t FIVE = 5;
//        uint8_t SIX = 6;
//        uint8_t SEVEN = 7;
//        uint8_t EIGHT = 8;
//        uint8_t NINE = 9;
//        uint8_t NOTHING = 10;
//        uint8_t SEG_A = 11;
//        uint8_t SEG_B = 12;
//        uint8_t SEG_C = 13;
//        uint8_t SEG_D = 14;
//        uint8_t SEG_E = 15;
//        uint8_t SEG_F = 16;
//        uint8_t SEG_G = 17;
//        uint8_t SEG_DP = 18;
//    };

    explicit SegmentDisplay(const uint8_t (*pins)[8], const uint8_t (*map)[19]) : _pins(pins), _map(map) {
        for (uint8_t i = 0; i < 8; i++) {
            const uint8_t p = pgm_read_byte(&_pins[0][i]);
            if (p) pinMode(p, OUTPUT);
        }
    }

    void render(const uint8_t symbol) {
        const uint8_t s = pgm_read_byte(&_map[0][symbol]);
        for (uint8_t i = 0; i < 8; i++) {
            const uint8_t p = pgm_read_byte(&_pins[0][i]);
            if (p) digitalWrite(p, (s & (1 << i)) != 0);
        }
    }

    void clear() {
        render(10);
    }

private:
    const uint8_t (*_pins)[8];

    const uint8_t (*_map)[19];
};

SegmentDisplay *sd;

void setup() {
    sd = new SegmentDisplay(&PINS, &MAP);
    sd->render(9);
}

void loop() {

}
