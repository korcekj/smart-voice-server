#ifndef REMOTE_H
#define REMOTE_H

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include "ESP8266_Modul.h"
#include <IRsend.h>
#include "Config.h"

class Remote : public ESP8266_Modul {
private:
    IRsend *irSend = nullptr;

    uint8_t esp8266Pin = DEFAULT_ESP8266_PIN;
    String name = "";
    uint8_t type = SONY_REMOTE;
    uint8_t frequency = DEFAULT_REMOTE_FREQUENCY;
    uint8_t command = -1;

    std::vector<void (Remote::*)()> typeFunctions = {&Remote::Sony};

    void Sony();

public:
    Remote();
    ~Remote();

    void init();
    void run();
    String toJSON();

    void parseProperties(String &, String &);

    void setEsp8266Pin(uint8_t);
    void setName(String);
    void setType(uint8_t);
    void setFrequency(uint8_t);
    void setCommand(uint8_t);

    uint8_t getEsp8266Pin();

};

#endif // REMOTE_H
