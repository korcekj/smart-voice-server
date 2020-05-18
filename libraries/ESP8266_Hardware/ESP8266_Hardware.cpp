#include "FirebaseJson.h"
#include "ESP8266_Hardware.h"
#include "Config.h"
#include <ctime>

ESP8266_Hardware::ESP8266_Hardware() {}

void ESP8266_Hardware::runHardware() {
    for (auto &pair : this->modules)
    {
        pair.second->run();
    }
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

bool ESP8266_Hardware::deleteModule(String &id) {
    ESP8266_Modul* module = this->getModule(id);
    if (module == nullptr)
        return false;
    
    delete module;
    this->modules.erase(id);
    return true;
}

ESP8266_Modul *ESP8266_Hardware::getModule(String &id) {
    std::map<String, ESP8266_Modul *>::iterator it;
    it = this->modules.find(id);

    if (it == this->modules.end())
        return nullptr;

    return it->second;
}

bool ESP8266_Hardware::existsModule(String &id) {
    if (this->getModule(id) == nullptr)
        return false;
    return true;
}