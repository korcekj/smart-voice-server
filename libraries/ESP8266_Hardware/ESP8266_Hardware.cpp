#include "FirebaseJson.h"
#include "ESP8266_Hardware.h"
#include "Config.h"

ESP8266_Hardware::ESP8266_Hardware() {}

bool ESP8266_Hardware::initLed(String id, String jsonData, bool create) {
    if (create && !this->existsLed(id)) {
        Led led = Led();
        this->parseJsonToData(jsonData, "led", false, &led, &ESP8266_Hardware::parseLedProperties);
        this->leds.insert(std::pair<String, Led>(id, led));
        return true;
    } else if (this->existsLed(id)) {
        Led *led = this->getLed(id);
        this->parseJsonToData(jsonData, "led", false, led, &ESP8266_Hardware::parseLedProperties);
        return true;
    }

    return false;
}

bool ESP8266_Hardware::requiredLed(String jsonData) {
    FirebaseJson json;
    json.setJsonData(jsonData);

    String name = json.parse().get("name").parseResult().stringValue;
    int numLeds = json.parse().get("numLeds").parseResult().intValue;

    if (name == "" || numLeds == 0)
        return false;
    return true;
}

String ESP8266_Hardware::createLed(String jsonData) {
    if (!this->requiredLed(jsonData)) 
        return "";

    String id = "led" + String(this->leds.size() + 1);
    bool success = this->initLed(id, jsonData, true);

    return success ? id : "";
}

void ESP8266_Hardware::parseJsonToData(String jsonData, String dataName, bool isEqual, Led *led, void (ESP8266_Hardware::*f)(String, String, void *)) {
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
        
        if (jsonParseResult.success) {
            if (isEqual && key.indexOf(dataName) >= 0)
                (this->*f)(key, value, led);
            else if (!isEqual && key.indexOf(dataName) == -1)
                (this->*f)(key, value, led);
        }
    }
}

void ESP8266_Hardware::parseLedProperties(String key, String value, void *l) {
    Led *led = (Led*)l;
    if (key == P_BRIGHTNESS)
        led->setBrightnessOfStrip(value.toInt());
    else if (key == P_MODE)
        led->setActiveMode(value.toInt());
    else if (key == P_NAME)
        led->setName(value);
    else if (key == P_NUM_LEDS)
        led->setNumLedsOnStrip(value.toInt());
    else if (key == P_PIN)
        led->setEsp8266Pin(value.toInt());
    else if (key == P_STATUS)
        led->setStatusOfStrip(value.toInt());
    else if (key == P_WAIT)
        led->setWaitTime(value.toInt());
    else if (key == P_COLORS)
        this->parseJsonToData(value, "color", true, led, &ESP8266_Hardware::parseColorBytes);
}

void ESP8266_Hardware::parseColorBytes(String id, String jsonData, void *l) {
    Led *led = (Led*)l;
    FirebaseJson json;
    uint8_t r, g, b;

    json.setJsonData(jsonData);
    r = json.parse().get("r").parseResult().intValue;
    g = json.parse().get("g").parseResult().intValue;
    b = json.parse().get("b").parseResult().intValue;
    led->setColorBytes(r, g, b);
}

Led *ESP8266_Hardware::getLed(String key) {
    std::map<String, Led>::iterator it;
    it = this->leds.find(key);

    if (it == this->leds.end())
        return nullptr;

    return &it->second;
}

bool ESP8266_Hardware::existsLed(String key) {
    if (this->getLed(key) == nullptr)
        return false;
    return true;
}

String ESP8266_Hardware::getLeds() {
    String result = "";

    for (auto &led : this->leds)
    {
        result += led.second.toString() + "\n";
    }

    return result;
}