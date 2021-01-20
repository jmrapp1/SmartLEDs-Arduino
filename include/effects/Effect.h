//
// Created by Jon Rapp on 1/20/21.
//

#ifndef LUMEN_EFFECT_H
#define LUMEN_EFFECT_H

class Effect {
public:
    Effect(StateService &stateService, LightService &lightService);
    virtual void setup(int numLeds, CRGB *leds) = 0;
    virtual void loop(int numLeds, CRGB *leds) = 0;
    virtual void onCmd(char** nextCmd, int numLeds, CRGB *leds) = 0;

protected:
    StateService &_stateService;
    LightService &_lightService;
};

#endif //LUMEN_EFFECT_H
