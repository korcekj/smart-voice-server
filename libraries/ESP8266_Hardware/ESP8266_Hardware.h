#ifndef ESP8266_Hardware_H
#define ESP8266_Hardware_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <IRsend.h>
#include "Led.h"
#include "Remote.h"
#include <map>
#include <vector>

class ESP8266_Hardware
{
private:
    std::vector<Adafruit_NeoPixel *> strips;
    std::vector<IRsend *> irSends;
    std::map<String, Led> leds;
    std::map<String, Remote> remotes;

    void parseJsonToData(String, String, bool, void *, void (ESP8266_Hardware::*)(String &, String &, void *));
    void parseLedProperties(String &, String &, void *);
    void parseRemoteProperties(String &, String &, void *);
    void parseColorBytes(String &, String &, void *);

    bool containParams(std::vector<String> &, String &);

public:
    ESP8266_Hardware();

    void addStrip(Adafruit_NeoPixel *);
    Adafruit_NeoPixel *getStrip(uint8_t);
    void addIRsend(IRsend *);
    IRsend *getIRsend(uint8_t);

    bool existsLed(String &);
    bool initLed(String, String, bool);
    bool deleteLed(String &);

    bool existsRemote(String &);
    bool initRemote(String, String, bool);

    const String createHardware(String &, std::vector<String> &, bool (ESP8266_Hardware::*)(String, String, bool));
    const bool updateHardware(String &, String &, bool (ESP8266_Hardware::*)(String, String, bool));
    const bool deleteHardware(String &, bool (ESP8266_Hardware::*)(String &));
    void runHardware();

    Led *getLed(String &);
    String getLeds();

    Remote *getRemote(String &);
    String getRemotes();
};

#endif // ESP8266_Hardware_H
