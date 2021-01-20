
#ifndef LUMEN_RFSERVICE_H
#define LUMEN_RFSERVICE_H

#include <RF24.h>
#include <EEPROM.h>

class RfService {
public:
    RfService();
    void setup();
    bool isCmdAvailable();
    char** getNextCmd();
    void loop();

private:
    void loadEepromRadioAddr();
    void clearNextCmd();
    void parseCmd(char c[]);

    RF24 _radio = RF24(9, 10);
    byte _radioAddr[5] = {0x31, 0x4E, 0x6F, 0x64, 0xFF}; // last byte gets replaced

    bool _newCmd = false;
    byte _nextCmdLength = 10; // max params
    char **_nextCmd = static_cast<char **>(malloc(_nextCmdLength * sizeof(char)));
};

#endif //LUMEN_RFSERVICE_H
