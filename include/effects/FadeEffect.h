
#ifndef LUMEN_FADEEFFECT_H
#define LUMEN_FADEEFFECT_H

#include <Arduino.h>
#include <FastLED.h>
#include <StateService.h>
#include <LightService.h>
#include "Effect.h"

class FadeEffect: public Effect {
public:
    FadeEffect(StateService &stateService, LightService &lightService) : Effect(stateService, lightService) {
    }
    void setup(int numLeds, CRGB *leds) override;
    void loop(int numLeds, CRGB *leds) override;
    void onCmd(char **nextCmd, int numLeds, CRGB *leds) override;

private:
    bool _downFade = true;
    byte _lastBrightness = 0;
    byte _minBrightness = 0;
    byte _maxBrightness = 0;
    byte _delay = 5;
};

#endif //LUMEN_FADEEFFECT_H
