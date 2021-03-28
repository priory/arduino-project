#include <Arduino.h>
#include <avr/pgmspace.h>
#include "SegmentDisplay.h"

SegmentDisplay::SegmentDisplay(const uint8_t (*pins)[8], const uint8_t (*map)[19]) : _pins(pins), _map(map) {
    for (uint8_t i = 0; i < NUM_OF_SEGMENTS; i++) {
        const uint8_t p = pgm_read_byte(&_pins[0][i]);
        if (p) pinMode(p, OUTPUT);
    }
}

void SegmentDisplay::render(const uint8_t symbol) {
    const uint8_t s = pgm_read_byte(&_map[0][symbol]);
    renderBits(s);
}

void SegmentDisplay::renderBits(const uint8_t v) {
    for (uint8_t i = 0; i < NUM_OF_SEGMENTS; i++) {
        segment(i, v >> i & 1);
    }
}

void SegmentDisplay::point(bool enabled) {
    segment(NUM_OF_SEGMENTS - 1, enabled);
}

void SegmentDisplay::segment(uint8_t segment, bool enabled) {
    const uint8_t p = pgm_read_byte(&_pins[0][segment]);
    if (p) digitalWrite(p, enabled);
}

void SegmentDisplay::clear() {
    renderBits(0b00000000);
}
