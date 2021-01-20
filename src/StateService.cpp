#include <StateService.h>

char *StateService::getState() {
    return _state;
}

bool StateService::isStateEqualTo(const char *c) {
    return strcmp(_state, c) == 0;
}

void StateService::setState(const char *state, byte length) {
    memcpy(_state, state, length);
    Serial.print("State: ");
    Serial.println(_state);
}
