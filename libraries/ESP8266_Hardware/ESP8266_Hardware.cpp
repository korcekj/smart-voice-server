#include "FirebaseJson.h"
#include "ESP8266_Hardware.h"
#include "Config.h"
#include <ctime>

ESP8266_Hardware::ESP8266_Hardware() {}

bool ESP8266_Hardware::initLed(String id, String jsonData, bool create) {
    if (create && !this->existsLed(id)) {
        Led led = Led();
        this->parseJsonToData(jsonData, "led", false, &led, &ESP8266_Hardware::parseLedProperties);
        this->leds.insert(std::pair<String, Led>(id, led));
        return true;
    } else if (this->existsLed(id)) {
        Led *led = this->getLed(id);
        led->clearColors();
        this->parseJsonToData(jsonData, "led", false, led, &ESP8266_Hardware::parseLedProperties);
        return true;
    }

    return false;
}

bool ESP8266_Hardware::containParams(std::vector<String> &requiredParams, String &jsonData) {
    if (requiredParams.empty()) return true;

    FirebaseJson json;
    json.setJsonData(jsonData);

    for (auto &requiredParam : requiredParams)
    {
        String param = json.parse().get(requiredParam).parseResult().stringValue;
        if (!param || param == "") return false;
    }
    
    return true;
}

const String ESP8266_Hardware::createHardware(String &jsonData, std::vector<String> &requiredParams, bool (ESP8266_Hardware::*f)(String, String, bool)) {
    if (!this->containParams(requiredParams, jsonData)) return "";

    std::time_t timestamp = std::time(0);
    String id = String(timestamp);
    bool success = (this->*f)(id, jsonData, true);

    return success ? id : "";
}

const bool ESP8266_Hardware::updateHardware(String &id, String &jsonData, bool (ESP8266_Hardware::*f)(String, String, bool)) {
    if (!id.length() || !jsonData.length()) return false;

    return (this->*f)(id, jsonData, false);
}

const bool ESP8266_Hardware::deleteHardware(String &id, bool (ESP8266_Hardware::*f)(String &)) {
    if (!id.length()) return false;

    return (this->*f)(id);
}

void ESP8266_Hardware::parseJsonToData(String jsonData, String dataName, bool isEqual, void *hardware, void (ESP8266_Hardware::*f)(String &, String &, void *)) {
    String key, value;

    FirebaseJson json;
    FirebaseJsonObject jsonParseResult;
    size_t count = 0;

    json.setJsonData(jsonData);
    count = json.parse().getJsonObjectIteratorCount();

    for (size_t i = 0; i < count; i++)
    {
        json.jsonObjectiterator(i, key, value);
        jsonParseResult = json.parseResult();
        
        if (jsonParseResult.success) {
            if (isEqual && key.indexOf(dataName) >= 0)
                (this->*f)(key, value, hardware);
            else if (!isEqual && key.indexOf(dataName) == -1)
                (this->*f)(key, value, hardware);
        }
    }
}

void ESP8266_Hardware::parseLedProperties(String &key, String &value, void *l) {
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

void ESP8266_Hardware::parseColorBytes(String &id, String &jsonData, void *l) {
    Led *led = (Led*)l;
    FirebaseJson json;
    uint8_t r, g, b;

    json.setJsonData(jsonData);
    r = json.parse().get("r").parseResult().intValue;
    g = json.parse().get("g").parseResult().intValue;
    b = json.parse().get("b").parseResult().intValue;
    led->setColorBytes(r, g, b);
}

bool ESP8266_Hardware::deleteLed(String &id) {
    if (!this->existsLed(id))
        return false;
    
    this->leds.erase(id);
    return true;
}

Led *ESP8266_Hardware::getLed(String &id) {
    std::map<String, Led>::iterator it;
    it = this->leds.find(id);

    if (it == this->leds.end())
        return nullptr;

    return &it->second;
}

bool ESP8266_Hardware::existsLed(String &id) {
    if (this->getLed(id) == nullptr)
        return false;
    return true;
}

String ESP8266_Hardware::getLeds() {
    String result = "";

    for (auto &led : this->leds)
    {
        result += led.second.toJSON() + "\n";
    }

    return result;
}