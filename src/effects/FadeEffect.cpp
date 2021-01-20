
#include <effects/FadeEffect.h>

void FadeEffect::setup(int numLeds, CRGB *leds) {
    _lastBrightness = Helpers::clampColor(255);
    FastLED.setBrightness(_lastBrightness);
}

void FadeEffect::loop(int numLeds, CRGB *leds) {
    if (_downFade && _lastBrightness <= _minBrightness + 1) {
        _downFade = false;
    } else if (!_downFade && _lastBrightness >= _maxBrightness) {
        _downFade = true;
    }

    if (_downFade) {
        _lastBrightness -= 1;
        FastLED.setBrightness(_lastBrightness);
    } else {
        _lastBrightness += 1;
        FastLED.setBrightness(_lastBrightness);
    }
    delay(_delay);
}

void FadeEffect::onCmd(char **nextCmd, int numLeds, CRGB *leds) {
    if (strcmp(nextCmd[2], "on") == 0) {
        _stateService.setState("FADE", 4);
        setup(numLeds, leds);
    }
    if (strcmp(nextCmd[2], "on") == 0 || strcmp(nextCmd[2], "p") == 0) { // p for params
        Helpers::parseNum(&_minBrightness, nextCmd[3]);
        Helpers::parseNum(&_maxBrightness, nextCmd[4]);
        Helpers::parseNum(&_delay, nextCmd[5]);
        if (_delay <= 0) _delay = 1;
        if (_minBrightness <= 0) _minBrightness = 0;
        if (_maxBrightness > 255) _maxBrightness = 255;
        _minBrightness = Helpers::clampColor(_minBrightness);
        _maxBrightness = Helpers::clampColor(_maxBrightness);
        Serial.println(_minBrightness);
        Serial.println(_maxBrightness);
        Serial.println(_delay);
    } else if (strcmp(nextCmd[2], "off") == 0) {
        _stateService.setState("IDLE", 4);
    } else if (strcmp(nextCmd[2], "c") == 0) { // c for color
        _lightService.setColor(nextCmd[3], nextCmd[4], nextCmd[5]);
    }
}
