
#include <utils/Helpers.h>
#include <LightService.h>

byte Helpers::clampColor(byte color) {
    return color * MAX_BRIGHTNESS_PERCENT;
}

void Helpers::parseNum(byte *out, char *s) {
    char *end;
    long l = strtol(s, &end, 10);
    *out = l;
}

void Helpers::subWithoutOverflow(byte *out, byte left, byte right) {
    if (left <= right) {
        Serial.println("Sub overflow");
        *out = 0;
    }
    *out = left - right;
}

void Helpers::addWithoutOverflow(byte *out, byte left, byte right) {
    if (left > 0 && right > 0 && (left + right <= left || left + right <= right)) {
        Serial.println("Add overflow");
        *out = 255;
    }
    *out = left + right;
}
