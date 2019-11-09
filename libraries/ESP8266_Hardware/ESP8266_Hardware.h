#ifndef ESP8266_Hardware_H
#define ESP8266_Hardware_H

#include <Arduino.h>
#include "Led.h"
#include <map>

class ESP8266_Hardware
{
private:
    std::map<String, Led> leds;

    void parseJsonToData(String, String, bool, Led *, void (ESP8266_Hardware::*)(String, String, void *));
    void parseLedProperties(String, String, void *);
    void parseColorBytes(String, String, void *);

    bool existsLed(String);
    bool requiredLed(String);

public:
    ESP8266_Hardware();

    bool initLed(String, String, bool);
    String createLed(String);

    Led *getLed(String);
    String getLeds();
};

#endif // ESP8266_Hardware_H
