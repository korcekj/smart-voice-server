#ifndef ESP8266_Hardware_H
#define ESP8266_Hardware_H

#include <Arduino.h>
#include "Led.h"
#include <map>
#include <vector>

class ESP8266_Hardware
{
private:
    std::map<String, Led> leds;

    void parseJsonToData(String, String, bool, void *, void (ESP8266_Hardware::*)(String &, String &, void *));
    void parseLedProperties(String &, String &, void *);
    void parseColorBytes(String &, String &, void *);

    bool containParams(std::vector<String> &, String &);

public:
    ESP8266_Hardware();

    bool existsLed(String &);
    bool initLed(String, String, bool);
    bool deleteLed(String &);

    const String createHardware(String &, std::vector<String> &, bool (ESP8266_Hardware::*)(String, String, bool));
    const String deleteHardware(String &, std::vector<String> &, bool (ESP8266_Hardware::*)(String &));

    Led *getLed(String &);
    String getLeds();
};

#endif // ESP8266_Hardware_H
