
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

