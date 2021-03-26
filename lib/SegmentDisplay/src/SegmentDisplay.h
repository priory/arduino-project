#ifndef SEGMENTDISPLAY_H
#define SEGMENTDISPLAY_H

#include <Arduino.h>

#define NUM_OF_SEGMENTS 8
#define NUM_OF_SYMBOLS 19

class SegmentDisplay {
public:
    /**
     * Constructor.
     *
     * @param pins: pointer to pins array
     * @param map: pointer to rendering map
     */
    explicit SegmentDisplay(const uint8_t (*pins)[NUM_OF_SEGMENTS], const uint8_t (*map)[19]);

    /**
     * Renders the symbol.
     *
     * @param symbol: symbol index from map PROGMEM
     */
    void render(uint8_t symbol);

    /**
     * Renders the segments based on 8-bit value.
     *
     *    PGFE DCBA
     * 0b 0000 0000
     *
     * @param v: value in bits
     */
    void renderBits(uint8_t v);

    /**
     * Render or turn off Decimal Point.
     *
     * @param enabled
     */
    void point(bool enabled = true);

    /**
     * Render or turn off a segment.
     *
     * @param segment: index of the pins array pointer
     * @param enabled
     */
    void segment(uint8_t segment, bool enabled = true);

    /**
     * Clears the segment display.
     */
    void clear();

private:
    const uint8_t (*_pins)[NUM_OF_SEGMENTS];

    const uint8_t (*_map)[NUM_OF_SYMBOLS];
};

#endif //SEGMENTDISPLAY_H
