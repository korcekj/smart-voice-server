#ifndef ESP8266_Hardware_H
#define ESP8266_Hardware_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "ESP8266_Modul.h"
#include <IRsend.h>
#include "Led.h"
#include "Remote.h"
#include <map>
#include <vector> 

class ESP8266_Hardware
{
private:
    std::map<String, ESP8266_Modul *> modules;

    bool containParams(std::vector<String> &, String &);

public:
    ESP8266_Hardware();

    const String createHardware(String &, std::vector<String> &, bool (ESP8266_Hardware::*)(String, String, bool));
    const bool updateHardware(String &, String &, bool (ESP8266_Hardware::*)(String, String, bool));
    const bool deleteHardware(String &, bool (ESP8266_Hardware::*)(String &));
    void runHardware();

    template <class MODULE>
    bool initModule(String, String, bool);
    bool existsModule(String &);
    ESP8266_Modul *getModule(String &);
    bool deleteModule(String &);
};

#include "ESP8266_Hardware.tcc"

#endif // ESP8266_Hardware_H
