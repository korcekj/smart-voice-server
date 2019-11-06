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

public:
    ESP8266_Hardware();

    void initLed(String, String);
    String createLed(String);

    String getLed(String);
    String getLeds();
};

#endif // ESP8266_Hardware_H
