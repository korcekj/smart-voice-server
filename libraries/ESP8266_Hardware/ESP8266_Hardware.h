#ifndef ESP8266_Hardware_H
#define ESP8266_Hardware_H

#include <Arduino.h>
#include "Led.h"
#include <map>

class ESP8266_Hardware
{
private:
    std::map<String, Led> leds;

    void initLed(String, String, Led *);
    void parseJsonToData(String, String, bool, Led *, void (ESP8266_Hardware::*)(String, String, Led *));
    void parseLedProperties(String, String, Led *);

public:
    ESP8266_Hardware();

    void initLeds(String);

    String getLed(String);
    String getLeds();

};

#endif // ESP8266_Hardware_H
