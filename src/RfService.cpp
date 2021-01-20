#include "RfService.h"

RfService::RfService() {
}

void RfService::loadEepromRadioAddr() {
    byte writtenFlag = EEPROM.read(1);
    if (writtenFlag != 255) {
        byte addr = EEPROM.read(0);
        Serial.print("Stored: ");
        Serial.println(addr);
        _radioAddr[4] = addr;
        return;
    }

    byte addr = 0;
    bool found = false;
    while (!found) {
        if (found || addr == 255) {
            break;
        }
        _radioAddr[4] = addr;
        _radio.openWritingPipe(_radioAddr);
        if (!_radio.write(&addr, sizeof(byte))) { // check if somethings on address listening
            Serial.print("Found: ");
            Serial.println(addr);
            found = true;
        } else {
            addr++;
        }
    }
    if (found) {
        EEPROM.write(0, addr);
        EEPROM.write(1, 1);
    }
}

void RfService::setup() {
    _radio.begin();
    _radio.setPALevel(RF24_PA_MAX);
    _radio.setChannel(0x4C);
    loadEepromRadioAddr();
    _radio.openReadingPipe(1, *(uint64_t *) _radioAddr);
    _radio.startListening();
}

bool RfService::isCmdAvailable() {
    return _newCmd;
}

void RfService::clearNextCmd() {
    for (byte i = 0; i < _nextCmdLength; i++) {
        _nextCmd[i] = nullptr;
    }
}

void RfService::parseCmd(char c[]) {
    _newCmd = true;
    clearNextCmd();
    char *ptr = strtok(c, {"~"});
    byte i = 0;
    while (ptr != nullptr && i < 10) {
        _nextCmd[i] = ptr;
        i++;
        ptr = strtok(nullptr, {"~"});
    }
}

char **RfService::getNextCmd() {
    _newCmd = false;
    return _nextCmd;
}

void RfService::loop() {
    if (_radio.available()) {
        Serial.println("Got");
        char msg[32] = {0};
        _radio.read(msg, sizeof(msg));
        Serial.println(msg);

        parseCmd(msg);
    }
}
