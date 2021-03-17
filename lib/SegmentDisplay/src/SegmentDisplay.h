#ifndef SEGMENTDISPLAY_H
#define SEGMENTDISPLAY_H

#include <Arduino.h>

#define NUM_OF_SEGMENTS 8
#define NUM_OF_SYMBOLS 19

class SegmentDisplay {
public:
    explicit SegmentDisplay(const uint8_t (*pins)[NUM_OF_SEGMENTS], const uint8_t (*map)[19]);

    void render(uint8_t symbol);

    void renderBits(uint8_t v);

    void point(bool enabled = true);

    void segment(uint8_t segment, bool enabled = true);

    void clear();

private:
    const uint8_t (*_pins)[NUM_OF_SEGMENTS];

    const uint8_t (*_map)[NUM_OF_SYMBOLS];
};

#endif //SEGMENTDISPLAY_H
