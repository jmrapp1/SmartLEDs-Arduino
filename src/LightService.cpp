
#include <LightService.h>

LightService::LightService(int numLeds, CRGB *leds) {
    _numLeds = numLeds;
    _leds = leds;
}

void LightService::setColor(byte r, byte g, byte b) {
    for (int i = 0; i < _numLeds; i++) {
        _leds[i] = CRGB(Helpers::clampColor(r), Helpers::clampColor(g), Helpers::clampColor(b));
    }
}

void LightService::setColor(char *r, char *g, char *b) {
    byte rB, gB, bB;
    Helpers::parseNum(&rB, r);
    Helpers::parseNum(&gB, g);
    Helpers::parseNum(&bB, b);
    setColor(rB, gB, bB);
}