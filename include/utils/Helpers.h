
#ifndef LUMEN_HELPERS_H
#define LUMEN_HELPERS_H

#import <Arduino.h>

class Helpers {
public:
    static byte clampColor(byte color);
    static void parseNum(byte *out, char *s);
    static void subWithoutOverflow(byte *out, byte left, byte right);
    static void addWithoutOverflow(byte *out, byte left, byte right);
};

#endif //LUMEN_HELPERS_H
