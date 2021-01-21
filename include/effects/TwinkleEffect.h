
#ifndef LUMEN_TWINKLEEFFECT_H
#define LUMEN_TWINKLEEFFECT_H

#include <Arduino.h>
#include <FastLED.h>
#include <StateService.h>
#include <LightService.h>
#include "Effect.h"

class TwinkleEffect: public Effect {
public:
    explicit TwinkleEffect(StateService &stateService, LightService &lightService, int numLeds);
    void setup(int numLeds, CRGB *leds) override;
    void loop(int numLeds, CRGB *leds) override;
    void onCmd(char **nextCmd, int numLeds, CRGB *leds) override;
    void startLed(CRGB *led);
    void twinkleLed(CRGB *led, int index);

private:
    bool *_ledDownFades; // if led is downfading
    byte *_ledSteps;

    byte _rChange, _gChange, _bChange; // color change
    byte _targetR, _targetG, _targetB; // target color
    byte _minR, _minG, _minB;

    byte _ledsStarted; // Number of leds started
    unsigned int _nextStartDelay; // Random delay to start next led

    bool isLedStarted(CRGB *led);
};

#endif //LUMEN_TWINKLEEFFECT_H
