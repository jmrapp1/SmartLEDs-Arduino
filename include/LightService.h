
#ifndef LUMEN_LIGHTSERVICE_H
#define LUMEN_LIGHTSERVICE_H

#include <Arduino.h>
#include <FastLED.h>

#define MAX_BRIGHTNESS 204
#define MAX_BRIGHTNESS_PERCENT MAX_BRIGHTNESS / 255

class LightService {
public:
    LightService(int numLeds, CRGB *leds);
    void setColor(byte r, byte g, byte b);
    void setColor(char* r, char* g, char* b);

private:
    int _numLeds;
    CRGB *_leds;

    void parseNum(byte *out, char *s);
    byte clampColor(byte color);
};

#endif //LUMEN_LIGHTSERVICE_H
