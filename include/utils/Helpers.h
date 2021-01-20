
#ifndef LUMEN_HELPERS_H
#define LUMEN_HELPERS_H

#import <Arduino.h>

class Helpers {
public:
    static byte clampColor(byte color);
    static void parseNum(byte *out, char *s);
};

#endif //LUMEN_HELPERS_H
