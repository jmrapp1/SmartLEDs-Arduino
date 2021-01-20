
#include <LightService.h>

LightService::LightService(int numLeds, CRGB *leds) {
    _numLeds = numLeds;
    _leds = leds;
}

void LightService::setColor(byte r, byte g, byte b) {
    for (int i = 0; i < _numLeds; i++) {
        _leds[i] = CRGB(clampColor(r), clampColor(g), clampColor(b));
    }
}

void LightService::setColor(char *r, char *g, char *b) {
    byte rB, gB, bB;
    parseNum(&rB, r);
    parseNum(&gB, g);
    parseNum(&bB, b);
    setColor(rB, gB, bB);
}

void LightService::parseNum(byte *out, char *s) {
    char *end;
    long l = strtol(s, &end, 10);
    *out = l;
}

byte LightService::clampColor(byte color) {
    return color * MAX_BRIGHTNESS_PERCENT;
}
