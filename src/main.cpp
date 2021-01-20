#include <Arduino.h>

#include <FastLED.h>
#include <RfService.h>
#include <StateService.h>
#include <effects/TwinkleEffect.h>
#include <LightService.h>
#include <effects/FadeEffect.h>

#define SKIP_PIXEL true
#define SKIP_PIXEL_AMOUNT 1
#define NUM_LEDS 254
#define DATA_PIN 6

// Define the array of leds
CRGB leds[NUM_LEDS];

RfService rfService = RfService();
StateService stateService = StateService();
LightService lightService = LightService(NUM_LEDS, leds);
TwinkleEffect twinkleEffect = TwinkleEffect(stateService, lightService, NUM_LEDS);
FadeEffect fadeEffect = FadeEffect(stateService, lightService);

void setupLights() {
    FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(MAX_BRIGHTNESS);
}

void setup() {
    delay(3000);
    Serial.begin(9600);
    setupLights();
    rfService.setup();
    Serial.println("Setup");
    delay(3000);
}

void doLights() {
    if (stateService.isStateEqualTo("TWINKLE")) {
        twinkleEffect.loop(NUM_LEDS, leds);
    } else if (stateService.isStateEqualTo("FADE")) {
        fadeEffect.loop(NUM_LEDS, leds);
    }
}

void handleNextCmd(char** nextCmd) {
    char *cmd = nextCmd[0];
    if (strcmp(cmd, "on") == 0) {
        FastLED.setBrightness(MAX_BRIGHTNESS);
    } else if (strcmp(cmd, "off") == 0) {
        FastLED.setBrightness(0);
    } else if (strcmp(cmd, "set") == 0) {
        char *type = nextCmd[1];
        if (strcmp(type, "twinkle") == 0) {
            twinkleEffect.onCmd(nextCmd, NUM_LEDS, leds);
        } else if (strcmp(type, "color") == 0) {
            stateService.setState("COLOR", 5);
            lightService.setColor(nextCmd[2], nextCmd[3], nextCmd[4]);
        } else if (strcmp(type, "fade") == 0) {
            fadeEffect.onCmd(nextCmd, NUM_LEDS, leds);
        }
    }
}

void loop() {
    doLights();
    rfService.loop();
    if (rfService.isCmdAvailable()) {
        handleNextCmd(rfService.getNextCmd());
    }
    FastLED.show();
}