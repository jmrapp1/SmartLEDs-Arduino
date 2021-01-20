
#ifndef LUMEN_STATESERVICE_H
#define LUMEN_STATESERVICE_H

#include <Arduino.h>

class StateService {
public:
    char *getState();
    bool isStateEqualTo(const char *c);
    void setState(const char *state, byte length);

private:
    char _state[16] = {};
};

#endif //LUMEN_STATESERVICE_H
