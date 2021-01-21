#include <StateService.h>

char *StateService::getState() {
    return _state;
}

void StateService::clearState() {
    for (char &i : _state) {
        i = '\0';
    }
}

bool StateService::isStateEqualTo(const char *c) {
    return strcmp(_state, c) == 0;
}

void StateService::setState(const char *state, byte length) {
    clearState();
    memcpy(_state, state, length);
    Serial.print("State: ");
    Serial.println(_state);
}
