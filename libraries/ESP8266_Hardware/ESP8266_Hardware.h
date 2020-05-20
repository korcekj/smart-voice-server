#ifndef ESP8266_Hardware_H
#define ESP8266_Hardware_H

// Include potrebnych kniznic
#include <Arduino.h>
#include "FirebaseJson.h"
#include "ESP8266_Modul.h"
#include "Led.h"
#include "Remote.h"
#include <map>
#include <vector> 
#include <ctime>

// ESP8266_Hardware 
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
    void update();

    template <class MODULE>
    bool initModule(String, String, bool);
    bool existsModule(String &);
    ESP8266_Modul *getModule(String &);
    bool deleteModule(String &);
};

// Include template motod
#include "ESP8266_Hardware.tcc"

#endif // ESP8266_Hardware_H
