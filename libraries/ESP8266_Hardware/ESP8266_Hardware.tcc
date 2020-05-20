// Metoda sluziaca na inicializaciu modulu (create/update)
template <class MODULE>
bool ESP8266_Hardware::initModule(String id, String jsonData, bool create) {
    if (create && !this->existsModule(id)) {
        // Vytvirenie noveho modulu ak neexistuje
        MODULE *module = new MODULE();
        // Preparsovanie json dat a priradenie k premennych vo vnutri funkcie modulu
        module->parseJsonToData(jsonData, "NULL", false, module, &MODULE::parseProperties);
        // Inicializacia modulu
        module->init();
        // Pridanie modulu medzi existujuce a aktualne dostupne k pripojeniu na ESP8266 Wemos D1 mini
        this->modules.insert(std::pair<String, MODULE *>(id, module));
        return true;
    } else if (this->existsModule(id)) {
        // Najdenie existujuceho modulu
        MODULE *module = (MODULE *)this->getModule(id);
        // Preparsovanie json dat a priradenie k premennych vo vnutri funkcie modulu
        module->parseJsonToData(jsonData, "NULL", false, module, &MODULE::parseProperties);
        return true;
    }

    return false;
}