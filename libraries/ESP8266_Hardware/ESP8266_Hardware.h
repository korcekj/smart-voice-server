#ifndef ESP8266_Hardware_H
#define ESP8266_Hardware_H

#include <Arduino.h>
#include "Led.h"
#include <map>

class ESP8266_Hardware
{
private:
    std::map<String, Led> leds;

    void initLed(String, String);

public:
    ESP8266_Hardware();

    void initLeds(String);

    String getLed(String);

};

#endif // ESP8266_Hardware_H
