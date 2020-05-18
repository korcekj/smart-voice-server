template <class MODULE>
bool ESP8266_Hardware::initModule(String id, String jsonData, bool create) {
    if (create && !this->existsModule(id)) {
        MODULE *module = new MODULE();
        module->parseJsonToData(jsonData, " ", false, module, &MODULE::parseProperties);
        module->init();
        this->modules.insert(std::pair<String, MODULE *>(id, module));
        return true;
    } else if (this->existsModule(id)) {
        MODULE *module = (MODULE *)this->getModule(id);
        module->parseJsonToData(jsonData, " ", false, module, &MODULE::parseProperties);
        return true;
    }

    return false;
}