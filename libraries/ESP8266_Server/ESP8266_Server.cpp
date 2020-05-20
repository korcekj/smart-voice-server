#include "ESP8266_Server.h"

// FirebaseData objekt
FirebaseData firebaseData;

// Konstruktor
ESP8266_Server::ESP8266_Server(ESP8266WebServer *server, ESP8266_Hardware *hardware) {
    this->server = server;
    this->hardware = hardware;
}

// Metoda sluziaca na inizializaciu servera a jeho prislusnych komponentov
void ESP8266_Server::init() {
    this->serverInit();
    this->firebaseInit();
    this->hardwareInit();
}

// Metoda na inicializaciu WiFi pripojenia a beh servera
void ESP8266_Server::serverInit() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(STASSID, STAPSK);
    this->connect();
    this->runDns();
    this->server->begin();
}

// Metoda sluziaca na inicializaciu pripojenia sa k databaze
void ESP8266_Server::firebaseInit() {
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    this->setFirebaseRootPath();
}

// Metoda sluziaca k inicializacia hardveru
void ESP8266_Server::hardwareInit() {
    // Inicializacia Led
    this->hardwareInitByType(this->firebaseRootPath + "/hardware/led", &ESP8266_Hardware::initModule<Led>);
    // Inicializacia Remote
    this->hardwareInitByType(this->firebaseRootPath + "/hardware/remote", &ESP8266_Hardware::initModule<Remote>);
}

// Metoda sluziaca k inicializacii hardverovych dat z databazy podla zvoleneho typu
void ESP8266_Server::hardwareInitByType(String path, bool (ESP8266_Hardware::*f)(String, String, bool)) {
    // Ziskanie udajov z databazy
    if (Firebase.getShallowData(firebaseData, path)) {
        // V pripade ze sa nejedna o JSON tak nepokracuj dalej
        if (firebaseData.dataType() != "json") return;
        // FirebaseJson objekt
        FirebaseJson json;
        // Pocet vnorenych dat v json
        size_t count = 0;
        String key, value;

        // Nastavenie json dat
        json.setJsonData(firebaseData.jsonData());
        // Zsikanie poctu vnorenych dat
        count = json.parse().getJsonObjectIteratorCount();
        // Prejdenie vsetkych dat v json
        for (size_t i = 0; i < count; i++)
        {
            json.jsonObjectiterator(i, key, value);
            // Postupne ziskanie vnorenych dat a vykonanie prislusnej funkcie
            if (value == "true" && Firebase.getJSON(firebaseData, path + "/" + key))
                (this->hardware->*f)(key, firebaseData.jsonData(), true);
        }   
    }
}

// Metoda na inicializaciu pripojenia Wi-Fi modulu a ziskanie IP ako aj MAC adresy
void ESP8266_Server::connect() {
    while (WiFi.status() != WL_CONNECTED)
        delay(500);

    // Ziskanie IP a MAC adresy
    this->setIpAddress(WiFi.localIP());
    this->setMacAddress(WiFi.macAddress());

    // Aktualizacia udajov IP a MAC v databaze
    Firebase.setString(firebaseData, this->firebaseRootPath + "/data/url", this->getUrl());
    Firebase.setString(firebaseData, this->firebaseRootPath + "/data/ip", this->getIpAddress());
}

// Metoda sluziacia na beh DNS 
void ESP8266_Server::runDns() {
    // Nastavenie URL adresy
    String dns = STADNS_BASE;
    dns += "-" + this->getDashedMacAddress();

    if (MDNS.begin(dns)) {
        this->dnsRunning = true;
        this->url = dns;
    } else {
        this->dnsRunning = false;
        this->url = this->getIpAddress();
    }
}

// Metoda na akutalizaciu dat servera a handlovanie klientov
void ESP8266_Server::update() {
    this->server->handleClient();
    MDNS.update();
}

// Metoda na handlovanie domovskej, teda root stranky
void ESP8266_Server::handleRoot(String url, HTTPMethod method) {
    this->server->on(url, method, [=]() {
        this->sendResponse(HTTP_OK, "success", "ESP8266 API");
    });
}

// Metoda na handlovanie nenajdenej stranky a poskytnutie vhodneho vystupu klientovi
void ESP8266_Server::handleNotFound() {
    this->server->onNotFound([=]() {
        this->sendResponse(HTTP_NOT_FOUND, "error", "Operation was not found.");
    });
}

// Metoda na handlovanie vsetkych Led poziadaviek
void ESP8266_Server::handleLed() {
    this->handleCreateLed("/led/create", HTTP_POST);
    this->handleUpdateLed("/led/update", HTTP_POST);
    this->handleDeleteLed("/led/delete", HTTP_POST);
}

// Metoda na handlovanie vsetkych Remote poziadaviek
void ESP8266_Server::handleRemote() {
    this->handleCreateRemote("/remote/create", HTTP_POST);
    this->handleUpdateRemote("/remote/update", HTTP_POST);
    this->handleDeleteRemote("/remote/delete", HTTP_POST);
}

// Metoda na handlovanie vytvorenia Led
void ESP8266_Server::handleCreateLed(String url, HTTPMethod method) {
    this->server->on(url, method, [=]() {
        // Definovanie pozadovanych GET parametrov
        std::vector<String> requiredArgs = {"userId", "moduleId", "plain"};
        // Validacia GET parametrov a validacia pouzivatela, ci je vlastnikom ESP8266 WiFi mudulu
        if (this->containArgs(requiredArgs)) {
            if (this->isUserOwner(this->server->arg("userId"), this->server->arg("moduleId"))) {

                // Definovanie pozadovanych vstupnych dat v JSON formate
                std::vector<String> requiredParams = {"name", "numLeds", "pin"};
                String jsonData = this->server->arg("plain");
                // Vytvorenie Led
                String id = this->hardware->createHardware(jsonData, requiredParams, &ESP8266_Hardware::initModule<Led>);

                if (id.length()) {
                    String json = this->hardware->getModule(id)->toJSON();

                    // Vratenie ID vytvorenej Led klientovi
                    char returnValue[id.length() + 1];
                    id.toCharArray(returnValue, id.length() + 1);
                    // Ulozenie dat Led do databazy 
                    if (Firebase.setJSON(firebaseData, this->firebaseRootPath + "/hardware/led/" + id, FirebaseJson().setJsonData(json)))
                        this->sendResponse(HTTP_CREATED_OK, "success", "Operation was successfully done.", returnValue);
                    else 
                        this->sendResponse(HTTP_INTERNAL_SERVER_ERROR, "error", "Operation was not successfully done.");
                } else {
                    this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
                }

            }
            else {
                this->sendResponse(HTTP_NOT_AUTHORIZED, "error", "Operation was not authorized.");
            }
        } else {
            this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
        }
    });
}

// Metoda na handlovanie aktualizacie Led
void ESP8266_Server::handleUpdateLed(String url, HTTPMethod method) {
    this->server->on(url, method, [=]() {
        // Definovanie pozadovanych GET parametrov
        std::vector<String> requiredArgs = {"id", "userId", "moduleId", "plain"};
        // Validacia GET parametrov a validacia pouzivatela, ci je vlastnikom ESP8266 WiFi mudulu
        if (this->containArgs(requiredArgs)) {
            if (this->isUserOwner(this->server->arg("userId"), this->server->arg("moduleId"))) {

                String id = this->server->arg("id");
                String jsonData = this->server->arg("plain");
                // Aktualizacia dat Led
                bool updated = this->hardware->updateHardware(id, jsonData, &ESP8266_Hardware::initModule<Led>);
                
                if (updated) {
                    String json = this->hardware->getModule(id)->toJSON();
                    // Ulozenie dat Led do databazy
                    if (Firebase.updateNode(firebaseData, this->firebaseRootPath + "/hardware/led/" + id, FirebaseJson().setJsonData(json)))
                        this->sendResponse(HTTP_OK, "success", "Operation was successfully done.");
                    else 
                        this->sendResponse(HTTP_INTERNAL_SERVER_ERROR, "error", "Operation was not successfully done.");
                } else {
                    this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
                }

            } else {
                this->sendResponse(HTTP_NOT_AUTHORIZED, "error", "Operation was not authorized.");
            }
        } else {
            this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
        }
    });
}

// Metoda na handlovanie vymazania Led
void ESP8266_Server::handleDeleteLed(String url, HTTPMethod method) {
    this->server->on(url, method, [=]() {
        // Definovanie pozadovanych GET parametrov
        std::vector<String> requiredArgs = {"id", "userId", "moduleId"};
        // Validacia GET parametrov a validacia pouzivatela, ci je vlastnikom ESP8266 WiFi mudulu
        if (this->containArgs(requiredArgs)) {
            if (this->isUserOwner(this->server->arg("userId"), this->server->arg("moduleId"))) {

                String id = this->server->arg("id");
                // Zmazanie Led
                bool deleted = this->hardware->deleteHardware(id, &ESP8266_Hardware::deleteModule);
                
                if (deleted) {
                    // Zmazanie Led udajov z databazy
                    if (Firebase.deleteNode(firebaseData, this->firebaseRootPath + "/hardware/led/" + id))
                        this->sendResponse(HTTP_OK, "success", "Operation was successfully done.");
                    else 
                        this->sendResponse(HTTP_INTERNAL_SERVER_ERROR, "error", "Operation was not successfully done.");
                } else {
                    this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
                }

            } else {
                this->sendResponse(HTTP_NOT_AUTHORIZED, "error", "Operation was not authorized.");
            }
        } else {
            this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
        }
    });
}

// Metoda na handlovanie vytvorenia Remote
void ESP8266_Server::handleCreateRemote(String url, HTTPMethod method) {
    this->server->on(url, method, [=]() {
        // Definovanie pozadovanych GET parametrov
        std::vector<String> requiredArgs = {"userId", "moduleId", "plain"};
        // Validacia GET parametrov a validacia pouzivatela, ci je vlastnikom ESP8266 WiFi mudulu
        if (this->containArgs(requiredArgs)) {
            if (this->isUserOwner(this->server->arg("userId"), this->server->arg("moduleId"))) {
                
                // Definovanie pozadovanych vstupnych dat v JSON formate
                std::vector<String> requiredParams = {"pin", "name", "type", "frequency"};
                String jsonData = this->server->arg("plain");
                // Vytvorenie Remote
                String id = this->hardware->createHardware(jsonData, requiredParams, &ESP8266_Hardware::initModule<Remote>);

                if (id.length()) {
                    String json = this->hardware->getModule(id)->toJSON();

                    // Vratenie ID vytvoreneho Remote klientovi
                    char returnValue[id.length() + 1];
                    id.toCharArray(returnValue, id.length() + 1);
                    // Ulozenie Remote dat do databazy
                    if (Firebase.setJSON(firebaseData, this->firebaseRootPath + "/hardware/remote/" + id, FirebaseJson().setJsonData(json)))
                        this->sendResponse(HTTP_CREATED_OK, "success", "Operation was successfully done.", returnValue);
                    else 
                        this->sendResponse(HTTP_INTERNAL_SERVER_ERROR, "error", "Operation was not successfully done.");
                } else {
                    this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
                }

            }
            else {
                this->sendResponse(HTTP_NOT_AUTHORIZED, "error", "Operation was not authorized.");
            }
        } else {
            this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
        }
    });
}

// Metoda na handlovanie aktualizacie Remote
void ESP8266_Server::handleUpdateRemote(String url, HTTPMethod method) {
    this->server->on(url, method, [=]() {
        // Definovanie pozadovanych GET parametrov
        std::vector<String> requiredArgs = {"id", "userId", "moduleId", "plain"};
        // Validacia GET parametrov a validacia pouzivatela, ci je vlastnikom ESP8266 WiFi mudulu
        if (this->containArgs(requiredArgs)) {
            if (this->isUserOwner(this->server->arg("userId"), this->server->arg("moduleId"))) {

                String id = this->server->arg("id");
                String jsonData = this->server->arg("plain");
                // Aktualizacia Remote udajov (vykonanie prikazu)
                bool updated = this->hardware->updateHardware(id, jsonData, &ESP8266_Hardware::initModule<Remote>);
                
                if (updated) {
                    this->sendResponse(HTTP_OK, "success", "Operation was successfully done.");
                } else {
                    this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
                }

            } else {
                this->sendResponse(HTTP_NOT_AUTHORIZED, "error", "Operation was not authorized.");
            }
        } else {
            this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
        }
    });
}

// Metoda na handlovanie vymazania Remote
void ESP8266_Server::handleDeleteRemote(String url, HTTPMethod method) {
    this->server->on(url, method, [=]() {
        // Definovanie pozadovanych GET parametrov
        std::vector<String> requiredArgs = {"id", "userId", "moduleId"};
        // Validacia GET parametrov a validacia pouzivatela, ci je vlastnikom ESP8266 WiFi mudulu
        if (this->containArgs(requiredArgs)) {
            if (this->isUserOwner(this->server->arg("userId"), this->server->arg("moduleId"))) {

                String id = this->server->arg("id");
                // Zmazanie Remote objektu
                bool deleted = this->hardware->deleteHardware(id, &ESP8266_Hardware::deleteModule);
                
                if (deleted) {
                    // Zmazanie Remote udajov z databazy
                    if (Firebase.deleteNode(firebaseData, this->firebaseRootPath + "/hardware/remote/" + id))
                        this->sendResponse(HTTP_OK, "success", "Operation was successfully done.");
                    else 
                        this->sendResponse(HTTP_INTERNAL_SERVER_ERROR, "error", "Operation was not successfully done.");
                } else {
                    this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
                }

            } else {
                this->sendResponse(HTTP_NOT_AUTHORIZED, "error", "Operation was not authorized.");
            }
        } else {
            this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
        }
    });
}

// Metoda na overenie vlastnictva modulu pouzivatelom
bool ESP8266_Server::isUserOwner(const String &userId, const String &moduleId) {
    if (Firebase.getJSON(firebaseData, "/users/" + userId + "/modules/" + moduleId))
        return true;
    else
        return false;
}

// Metoda na overenie obsahu udajov
bool ESP8266_Server::containArgs(std::vector<String> &requiredArgs) {
    for (auto &requiredArg : requiredArgs)
    {
        if (!this->server->hasArg(requiredArg)) return false;
    }
    
    return true;
}

// Metoda sluziacia na vytvorenie odovede v JSON formate
void ESP8266_Server::createResponseJSON(char *json, int status, const char *rate, const char *message, const char *returnValue) {
    const int capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3);
    // JSON objekt
    StaticJsonDocument<capacity> doc;
    // Vyplnenie JSON poskytnutymi datami
    doc["status"] = status;
    if (returnValue && returnValue != "")
        doc["returnValue"] = returnValue;
    JsonObject description = doc.createNestedObject("description");
    description["rate"] = rate;
    description["message"] = message;
    // Serializacia JSON objektu
    serializeJson(doc, json, JSON_BUFF_SIZE);
}

// Metoda sluziacia na odoslanie odpovede klientovi v JSON formate
void ESP8266_Server::sendResponse(int status, const char *rate, const char *message, const char *returnValue) {
    char json[JSON_BUFF_SIZE];
    this->createResponseJSON(json, status, rate, message, returnValue);
    // Odoslanie odpovede klientovi
    this->server->send(status, "text/json", json);
}

// Metoda na nastevenie Firebase root cesty k: `this` modulu
void ESP8266_Server::setFirebaseRootPath() {
    this->firebaseRootPath = "/modules/" + this->getDashedMacAddress();
}

// Metoda na nastavenie IP adresy
void ESP8266_Server::setIpAddress(IPAddress ipAddress) {
    this->ip = ipAddress;
}

// Metoda na nastavenie MAC adresy
void ESP8266_Server::setMacAddress(String macAddress) {
    this->mac = macAddress;
}

// Metoda na ziskanie aktualnej IP adresy
String ESP8266_Server::getIpAddress() {
    return this->ip.toString();
}

// Metoda na ziskanie aktualnej MAC adresy
String ESP8266_Server::getMacAddress() {
    return this->mac;
}

// Metoda na ziskanie MAC adresy vo formate `XX-XX-XX-XX-XX-XX`
String ESP8266_Server::getDashedMacAddress() {
    String macAddress = this->mac;
    macAddress.replace(':', '-');
    return macAddress;
}

// Metoda na ziskanie root URL adresy ESP8266_Server
String ESP8266_Server::getUrl() {
    return this->url;
}
