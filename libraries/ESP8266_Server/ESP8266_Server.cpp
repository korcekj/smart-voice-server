#include "FirebaseESP8266.h"
#include "ESP8266_Server.h"
#include "Config.h"
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

// Firebase data
FirebaseData firebaseData;

ESP8266_Server::ESP8266_Server(ESP8266WebServer *server, ESP8266_Hardware *hardware) {
    this->server = server;
    this->hardware = hardware;
}

void ESP8266_Server::init() {
    this->serverInit();
    this->firebaseInit();
    this->hardwareInit();
}

void ESP8266_Server::serverInit() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(STASSID, STAPSK);
    this->connect();
    this->runDns();
    this->server->begin();
}

void ESP8266_Server::firebaseInit() {
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    this->setFirebaseRootPath();

    Firebase.setString(firebaseData, this->firebaseRootPath + "/data/url", this->url);
    Firebase.setString(firebaseData, this->firebaseRootPath + "/data/ip", this->getIpAddress());
}

void ESP8266_Server::hardwareInit() {
    this->hardwareInitByType(this->firebaseRootPath + "/hardware/led", &ESP8266_Hardware::initLed);
}

void ESP8266_Server::hardwareInitByType(String path, bool (ESP8266_Hardware::*f)(String, String, bool)) {
    if (Firebase.getShallowData(firebaseData, path)) {
        if (firebaseData.dataType() != "json") return;

        FirebaseJson json;
        size_t count = 0;
        String key, value;

        json.setJsonData(firebaseData.jsonData());
        count = json.parse().getJsonObjectIteratorCount();

        for (size_t i = 0; i < count; i++)
        {
            json.jsonObjectiterator(i, key, value);

            if (value == "true" && Firebase.getJSON(firebaseData, path + "/" + key))
                (this->hardware->*f)(key, firebaseData.jsonData(), true);
        }   
    }
}

void ESP8266_Server::connect() {
    while (WiFi.status() != WL_CONNECTED)
        delay(500);

    this->ip = WiFi.localIP();
    this->mac = WiFi.macAddress();
}

void ESP8266_Server::runDns() {
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

void ESP8266_Server::update() {
    this->server->handleClient();
    MDNS.update();
}

void ESP8266_Server::handleRoot(String url, HTTPMethod method) {
    this->server->on(url, method, [=]() {
        this->sendResponse(HTTP_OK, "success", "ESP8266 API");
    });
}

void ESP8266_Server::handleNotFound() {
    this->server->onNotFound([=]() {
        this->sendResponse(HTTP_NOT_FOUND, "error", "Operation was not found.");
    });
}

void ESP8266_Server::handleLed() {
    this->handleCreateLed("/led/create", HTTP_POST);
    this->handleUpdateLed("/led/update", HTTP_POST);
    this->handleDeleteLed("/led/delete", HTTP_POST);
}

void ESP8266_Server::handleCreateLed(String url, HTTPMethod method) {
    this->server->on(url, method, [=]() {
        std::vector<String> requiredArgs = {"userId", "moduleId", "plain"};
        
        if (this->containArgs(requiredArgs)) {
            if (this->isUserOwner(this->server->arg("userId"), this->server->arg("moduleId"))) {
                
                std::vector<String> requiredParams = {"name", "numLeds", "pin"};
                String jsonData = this->server->arg("plain");
                String id = this->hardware->createHardware(jsonData, requiredParams, &ESP8266_Hardware::initLed);

                if (id.length()) {
                    String json = this->hardware->getLed(id)->toJSON();

                    // Return created id back to the client
                    char returnValue[id.length() + 1];
                    id.toCharArray(returnValue, id.length() + 1);

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

void ESP8266_Server::handleUpdateLed(String url, HTTPMethod method) {
    this->server->on(url, method, [=]() {
        std::vector<String> requiredArgs = {"id", "userId", "moduleId", "plain"};
        
        if (this->containArgs(requiredArgs)) {
            if (this->isUserOwner(this->server->arg("userId"), this->server->arg("moduleId"))) {

                String id = this->server->arg("id");
                String jsonData = this->server->arg("plain");
                bool updated = this->hardware->updateHardware(id, jsonData, &ESP8266_Hardware::initLed);
                
                if (updated) {
                    String json = this->hardware->getLed(id)->toJSON();
                    
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

void ESP8266_Server::handleDeleteLed(String url, HTTPMethod method) {
    this->server->on(url, method, [=]() {
        std::vector<String> requiredArgs = {"id", "userId", "moduleId"};
        
        if (this->containArgs(requiredArgs)) {
            if (this->isUserOwner(this->server->arg("userId"), this->server->arg("moduleId"))) {

                String id = this->server->arg("id");
                bool deleted = this->hardware->deleteHardware(id, &ESP8266_Hardware::deleteLed);
                
                if (deleted) {
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

bool ESP8266_Server::isUserOwner(const String &userId, const String &moduleId) {
    if (Firebase.getJSON(firebaseData, "/users/" + userId + "/modules/" + moduleId))
        return true;
    else
        return false;
}

bool ESP8266_Server::containArgs(std::vector<String> &requiredArgs) {
    for (auto &requiredArg : requiredArgs)
    {
        if (!this->server->hasArg(requiredArg)) return false;
    }
    
    return true;
}

void ESP8266_Server::createResponseJSON(char *json, int status, const char *rate, const char *message, const char *returnValue) {
    const int capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3);
    StaticJsonDocument<capacity> doc;

    doc["status"] = status;
    if (returnValue && returnValue != "")
        doc["returnValue"] = returnValue;
    JsonObject description = doc.createNestedObject("description");
    description["rate"] = rate;
    description["message"] = message;
    serializeJson(doc, json, JSON_BUFF_SIZE);
}

void ESP8266_Server::sendResponse(int status, const char *rate, const char *message, const char *returnValue) {
    char json[JSON_BUFF_SIZE];
    this->createResponseJSON(json, status, rate, message, returnValue);

    this->server->send(status, "text/json", json);
}

void ESP8266_Server::setFirebaseRootPath() {
    this->firebaseRootPath = "/modules/" + this->getDashedMacAddress();
}

String ESP8266_Server::getIpAddress() {
    return this->ip.toString();
}

String ESP8266_Server::getMacAddress() {
    return this->mac;
}

String ESP8266_Server::getDashedMacAddress() {
    String macAddress = this->mac;
    macAddress.replace(':', '-');
    return macAddress;
}

String ESP8266_Server::getUrl() {
    return this->url;
}

String ESP8266_Server::getFirebaseError() {
    return this->firebaseError;
}
