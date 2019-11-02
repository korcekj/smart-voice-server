#include "FirebaseJson.h"
#include "ESP8266_Hardware.h"

ESP8266_Hardware::ESP8266_Hardware() {}

void ESP8266_Hardware::initLeds(String jsonData) {
    String key, value;

    FirebaseJson json;
    FirebaseJsonObject jsonParseResult;
    size_t count = 0;

    json.setJsonData(jsonData);
    count = json.parse().getJsonObjectIteratorCount();

    for (size_t i = 0; i < count; i++)
    {
        json.jsonObjectiterator(i,key,value);
        jsonParseResult = json.parseResult();
        
        if (jsonParseResult.success && jsonParseResult.type == "object")
            this->initLed(key, value);
    }
}

void ESP8266_Hardware::initLed(String id, String jsonData) {
    Led led = Led();

    String key, value;

    FirebaseJson json;
    FirebaseJsonObject jsonParseResult;
    size_t count = 0;

    json.setJsonData(jsonData);
    count = json.parse().getJsonObjectIteratorCount();

    for (size_t i = 0; i < count; i++)
    {
        json.jsonObjectiterator(i,key,value);
        jsonParseResult = json.parseResult();
        
        if (jsonParseResult.success && jsonParseResult.type != "object") {
            if (key ==  "brightness")
                led.setBrightnessOfStrip(jsonParseResult.intValue);
            else if (key ==  "mode")
                led.setActiveMode(jsonParseResult.intValue);
            else if (key ==  "name")
                led.setName(jsonParseResult.stringValue);
            else if (key ==  "numLeds")
                led.setNumLedsOnStrip(jsonParseResult.intValue);
            else if (key ==  "pin")
                led.setEsp8266Pin(jsonParseResult.intValue);
            else if (key ==  "status")
                led.setStatusOfStrip(jsonParseResult.intValue);
            else if (key ==  "wait")
                led.setWaitTime(jsonParseResult.intValue);
        }
    }

    this->leds.insert(std::pair<String, Led>(id, led));
}

String ESP8266_Hardware::getLed(String key) {
    std::map<String, Led>::iterator it;
    it = this->leds.find(key);

    if (it == this->leds.end())
        return "";
    
    return it->second.getName();
}