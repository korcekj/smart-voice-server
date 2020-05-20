#include "ESP8266_Hardware.h"

// Konstruktor
ESP8266_Hardware::ESP8266_Hardware() {}

// Metoda sluziaca na "beh" hardverovych sucasti v loop-e main programu
void ESP8266_Hardware::update() {
    for (auto &pair : this->modules)
    {
        pair.second->run();
    }
}

// Metoda sluziaca na kontrolu vstupnych premennych pri vytvarani hardverovej sucasti
bool ESP8266_Hardware::containParams(std::vector<String> &requiredParams, String &jsonData) {
    if (requiredParams.empty()) return true;

    // Firebase json objekt
    FirebaseJson json;
    // Nastavenie json dat objektu
    json.setJsonData(jsonData);

    // Kontrola pozadovanych dat na zaklade vstupnych dat: `jsonData`
    for (auto &requiredParam : requiredParams)
    {
        String param = json.parse().get(requiredParam).parseResult().stringValue;
        if (!param || param == "") return false;
    }
    
    return true;
}

// Metoda sluziaca na vytvorenie hardverovej sucasti
const String ESP8266_Hardware::createHardware(String &jsonData, std::vector<String> &requiredParams, bool (ESP8266_Hardware::*f)(String, String, bool)) {
    // Kontrola vstupnych parametrov
    if (!this->containParams(requiredParams, jsonData)) return "";
    // Vytvorenie unikatneho ID pre hardverovu sucast, v tomto pripade sa jedna o Timestamp
    std::time_t timestamp = std::time(0);
    String id = String(timestamp);
    // Vyhodnotenie uspesnosti vytvorenia hardveru
    bool success = (this->*f)(id, jsonData, true);
    // Vratenie ID alebo prazdneho String v pripade ze hardver nebol uspesne vytvoreny
    return success ? id : "";
}

// Metoda sluziaca na aktualizaciu hardverovej sucasti
const bool ESP8266_Hardware::updateHardware(String &id, String &jsonData, bool (ESP8266_Hardware::*f)(String, String, bool)) {
    // Validacia vstupnych dat
    if (!id.length() || !jsonData.length()) return false;

    // Vratenie uspesnosti aktualizacie hardverovej sucasti
    return (this->*f)(id, jsonData, false);
}

// Metoda sluziaca na zmazanie hardverovej sucasti
const bool ESP8266_Hardware::deleteHardware(String &id, bool (ESP8266_Hardware::*f)(String &)) {
    // Validacia vstupnych dat
    if (!id.length()) return false;

    // Vratenie uspesnosti zmazania hardverovej sucasti
    return (this->*f)(id);
}

// Metoda na zmazanie konkretneho modulu podla ID: `id`
bool ESP8266_Hardware::deleteModule(String &id) {
    // Overenie existencie modulu
    ESP8266_Module* module = this->getModule(id);
    if (module == nullptr)
        return false;
    
    // Zmazanie a uvolnenie blokov v pamati 
    delete module;
    this->modules.erase(id);
    return true;
}

// Metoda na ziskanie existujuceho modulu na zaklade ID: `id`
ESP8266_Module *ESP8266_Hardware::getModule(String &id) {
    // Najdenie modulu
    std::map<String, ESP8266_Module *>::iterator it;
    it = this->modules.find(id);

    // V pripade ze modul neexistuje, vratenie: `nullptr`
    if (it == this->modules.end())
        return nullptr;

    // Modul ako navratova hodnota metody
    return it->second;
}

// Metoda na overenie existencie modulu
bool ESP8266_Hardware::existsModule(String &id) {
    if (this->getModule(id) == nullptr)
        return false;
    return true;
}