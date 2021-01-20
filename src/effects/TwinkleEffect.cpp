#include <effects/TwinkleEffect.h>
#include <StateService.h>

#define SKIP_PIXEL false
#define SKIP_PIXEL_AMOUNT 1
#define FADE_DELAY 5
#define LED_LIMIT_HIGH 204
#define LED_LIMIT_LOW 0

TwinkleEffect::TwinkleEffect(StateService &stateService, LightService &lightService, int numLeds) : Effect(stateService,
                                                                                                         lightService) {
    _ledDownFades = static_cast<bool *>(malloc(numLeds * sizeof(bool)));
}

void TwinkleEffect::setup(int numLeds, CRGB *leds) {
    for (int i = 0; i < numLeds; i++) {
        _ledDownFades[i] = false;
        leds[i] = CRGB(0, 0, 0);
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

void TwinkleEffect::onCmd(char **nextCmd, int numLeds, CRGB *leds) {
    if (strcmp(nextCmd[2], "on") == 0) {
        _stateService.setState("TWINKLE", 7);
        setup(numLeds, leds);
    } else if (strcmp(nextCmd[2], "off") == 0) {
        _stateService.setState("IDLE", 4);
    } else if (_stateService.isStateEqualTo("TWINKLE")) {
        setup(numLeds, leds);
    }
}
