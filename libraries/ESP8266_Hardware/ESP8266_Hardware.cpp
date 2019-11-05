#include "FirebaseJson.h"
#include "ESP8266_Hardware.h"
#include "Config.h"

ESP8266_Hardware::ESP8266_Hardware() {}

void ESP8266_Hardware::initLeds(String jsonData) {
    this->parseJsonToData(jsonData, "led", true, nullptr, &ESP8266_Hardware::initLed);
}

void ESP8266_Hardware::parseJsonToData(String jsonData, String dataName, bool isEqual, Led *led, void (ESP8266_Hardware::*f)(String, String, Led *)) {
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

void ESP8266_Hardware::initLed(String id, String jsonData, Led *l) {
    Led led;

    if (l == nullptr)
        led = Led();
    else 
        led = *l;

    this->parseJsonToData(jsonData, "led", false, &led, &ESP8266_Hardware::parseLedProperties);
    this->leds.insert(std::pair<String, Led>(id, led));
}

void ESP8266_Hardware::parseLedProperties(String key, String value, Led *led) {
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
}

String ESP8266_Hardware::getLed(String key) {
    std::map<String, Led>::iterator it;
    it = this->leds.find(key);

    if (it == this->leds.end())
        return "";

    return it->second.toString();
}

String ESP8266_Hardware::getLeds() {
    String result = "";

    for (auto &led : this->leds)
    {
        result += led.second.toString() + "\n";
    }

    return result;
}