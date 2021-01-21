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
    _ledSteps = static_cast<byte *>(malloc(numLeds * sizeof(byte)));
}

/**
current  0   0   0
target  75  50 160
        ----------
        75  50 160
             ^  ^
             |  |
           min max

steps = max(1, max) = 160;

r: 75/160 = 0.46
g: 50/160 = 0.31
b: 160/160 = 1

rStep: 1/0.46 = round(2.17) = 2
gStep: 1/0.31 = round(3.22) = 3
bStep: 1/1    = round(1)    = 1

bool add = step % rStep == 0;

r: 75/160 = 0.4687
g: 50/160 = 0.3125 <--- min
b: 160/160 = 1

1 / 0.3125 = 3.2 <--- scalar

Changes:
r: 0.4687 * 3.2 = 1.499
g: 0.3125 * 3.2 = 1
b: 1.0000 * 3.2 = 3.2
 */
void TwinkleEffect::setup(int numLeds, CRGB *leds) {
    for (int i = 0; i < numLeds; i++) {
        _ledDownFades[i] = false;
        _ledSteps[i] = 0;
        leds[i] = CRGB(0, 0, 0);
    }
    double highest = max(_targetR, max(_targetG, _targetB));
    double rC = (double)_targetR / highest;
    double gC = (double)_targetG / highest;
    double bC = (double)_targetB / highest;

    double min = min(rC, min(gC, bC));
    if (min <= 0) min = 1;

    double scalar = 1 / min;
    _rChange = (byte) lround(max(scalar * rC, 255));
    _gChange = (byte) lround(max(scalar * gC, 255));
    _bChange = (byte) lround(max(scalar * bC, 255));

    _minR = (_targetR > 0 ? 1 : 0);
    _minG = (_targetG > 0 ? 1 : 0);
    _minB = (_targetB > 0 ? 1 : 0);

    Serial.println(_minR);
    Serial.println(_minG);
    Serial.println(_minB);
    Serial.println("");
    Serial.println(_rChange);
    Serial.println(_bChange);
    Serial.println(_gChange);
    Serial.println("");

    FastLED.show();
    delay(2000);
    _ledsStarted = 0;
    _nextStartDelay = 0;
}

void TwinkleEffect::loop(int numLeds, CRGB *leds) {
    if (_ledsStarted < numLeds) {
        EVERY_N_MILLIS(_nextStartDelay) {
            byte next = random(numLeds);
            if (!isLedStarted(&leds[next])) { // if not lit
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
    *led = CRGB(_minR, _minG, _minB);
    _ledSteps[index] = 1;
}

void TwinkleEffect::twinkleLed(CRGB *led, int index) {
    if (!isLedStarted(led)) { // if not lit
        return; // not started
    }

    if (!_ledDownFades[index] && (*led).r >= _targetR && (*led).g >= _targetG && (*led).b >= _targetB) {
        _ledDownFades[index] = true;
    } else if (_ledDownFades[index] && (*led).r <= _minR && (*led).g <= _minG && (*led).b <= _minB) {
        _ledDownFades[index] = false;
    }

    byte newR = (*led).r, newG = (*led).g, newB = (*led).b;
    if (_ledDownFades[index]) {
        if ((*led).r > _minR) Helpers::subWithoutOverflow(&newR, (*led).r, _rChange);
        if ((*led).g > _minG) Helpers::subWithoutOverflow(&newG, (*led).g, _gChange);
        if ((*led).b > _minB) Helpers::subWithoutOverflow(&newB, (*led).b, _bChange);
    } else {
        if ((*led).r < _targetR) Helpers::addWithoutOverflow(&newR, (*led).r, _rChange);
        if ((*led).g < _targetG) Helpers::addWithoutOverflow(&newG, (*led).g, _gChange);
        if ((*led).b < _targetB) Helpers::addWithoutOverflow(&newB, (*led).b, _bChange);
    }
    if (index == 1) {
        Serial.print("R ");
        Serial.println(newR);
        Serial.print("G ");
        Serial.println(newG);
        Serial.print("B ");
        Serial.println(newB);
    }
    (*led).r = constrain(newR, _minR, _targetR);
    (*led).g = constrain(newG, _minG, _targetG);
    (*led).b = constrain(newB, _minB, _targetB);
    _ledSteps[index] += 1;
}

bool TwinkleEffect::isLedStarted(CRGB *led) {
    return !((*led).r == 0 && (*led).g == 0 && (*led).b == 0);
}

void TwinkleEffect::onCmd(char **nextCmd, int numLeds, CRGB *leds) {
    if (strcmp(nextCmd[2], "on") == 0) {
        _stateService.setState("TWINKLE", 7);
        Helpers::parseNum(&_targetR, nextCmd[3]);
        Helpers::parseNum(&_targetG, nextCmd[4]);
        Helpers::parseNum(&_targetB, nextCmd[5]);
        FastLED.setBrightness(MAX_BRIGHTNESS);
        setup(numLeds, leds);
    } else if (strcmp(nextCmd[2], "off") == 0) {
        _stateService.setState("IDLE", 4);
    } else if (_stateService.isStateEqualTo("TWINKLE")) {
        setup(numLeds, leds);
    }
}
