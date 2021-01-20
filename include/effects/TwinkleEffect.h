
#ifndef LUMEN_TWINKLEEFFECT_H
#define LUMEN_TWINKLEEFFECT_H

#include <Arduino.h>
#include <FastLED.h>

class TwinkleEffect {
public:
    TwinkleEffect(int numLeds);
    void setup(int numLeds, CRGB *leds);
    void startLed(CRGB *led);
    void twinkleLed(CRGB *led, int index);
    void loop(int numLeds, CRGB *leds);

private:
    bool *_ledDownFades;
    byte _ledsStarted; // Number of leds started
    unsigned int _nextStartDelay; // Random delay to start next led
};

#endif //LUMEN_TWINKLEEFFECT_H
