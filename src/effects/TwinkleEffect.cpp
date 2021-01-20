#include <effects/TwinkleEffect.h>

#define SKIP_PIXEL false
#define SKIP_PIXEL_AMOUNT 1
#define FADE_DELAY 5
#define LED_LIMIT_HIGH 204
#define LED_LIMIT_LOW 0

TwinkleEffect::TwinkleEffect(int numLeds) {
    _ledDownFades = static_cast<bool *>(malloc(numLeds * sizeof(bool)));
}

void TwinkleEffect::setup(int numLeds, CRGB leds[]) {
    for (int i = 0; i < numLeds; i++) {
        _ledDownFades[i] = false;
    }
    FastLED.show();
    delay(2000);
    _ledsStarted = 0;
    _nextStartDelay = 0;
}

void TwinkleEffect::loop(int numLeds, CRGB *leds) {
    if (_ledsStarted < numLeds) {
        EVERY_N_MILLIS(_nextStartDelay) {
            byte next = random(numLeds);
            if (leds[next].r == 0) { // if not lit
                startLed(&leds[next]);
                _ledsStarted++;
                _nextStartDelay = random(500, 1500);
            } else {
                _nextStartDelay = 0;
            }
        }
    }

    for (int i = 0; i < numLeds; i++) {
        if (SKIP_PIXEL && i % SKIP_PIXEL_AMOUNT != 0) {
             leds[i] = CRGB::Black;
             continue;
        }
        twinkleLed(&leds[i], i);
    }
    delay(FADE_DELAY);
}

void TwinkleEffect::startLed(CRGB *led) {
    *led = CRGB(1, 1, 1);
}

void TwinkleEffect::twinkleLed(CRGB *led, int index) {
    if ((*led).r == 0) { // if not lit
        return; // not started
    }

    if ((*led).r >= LED_LIMIT_HIGH) {
        _ledDownFades[index] = true;
    } else if ((*led).r <= 1 + LED_LIMIT_LOW) { // 0 signals its not started so clamp at 1
        _ledDownFades[index] = false;
    }

    if (_ledDownFades[index]) {
        *led -= CRGB(1, 1, 1);
    } else {
        *led += CRGB(1, 1, 1);
    }
}
