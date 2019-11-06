#include "FirebaseESP8266.h"
#include "ESP8266_Server.h"
#include "Config.h"
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

// Firebase data
FirebaseData firebaseData;

ESP8266_Server::ESP8266_Server(ESP8266WebServer *server) {
    this->server = server;
    this->hardware = ESP8266_Hardware();
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
    //Set database read timeout to 1 minute (max 15 minutes)
    Firebase.setReadTimeout(firebaseData, 1000 * 60);
    Firebase.setString(firebaseData, this->firebaseRootPath + "/data/url", this->url);
}

void ESP8266_Server::hardwareInit() {
    this->hardwareLedsInit(this->firebaseRootPath + "/hardware/leds");
}

void ESP8266_Server::hardwareLedsInit(String path) {
    int id = 1;
    while(Firebase.getJSON(firebaseData, path + "/led" + String(id)))
        this->hardware.initLed("/led" + String(id++), firebaseData.jsonData());
    this->firebaseError = firebaseData.errorReason();
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
        this->url = this->getMacAddress();
    }
}

void ESP8266_Server::update() {
    this->server->handleClient();
    MDNS.update();
}

void ESP8266_Server::handleRoot(String url, HTTPMethod method) {
    this->server->on(url, method, [this]() {
        this->sendResponse(HTTP_OK, "success", "ESP8266 API");
    });
}

void ESP8266_Server::handleNotFound() {
    this->server->onNotFound([this]() {
        this->sendResponse(HTTP_NOT_FOUND, "error", "Operation was not found.");
    });
}

void ESP8266_Server::handleLed(String url) {
    this->handleCreateLed(url, HTTP_POST);
}

void ESP8266_Server::handleCreateLed(String url, HTTPMethod method) {
    this->server->on(url, method, [this]() {
        String jsonData = this->server->arg(0);
        String id = this->hardware.createLed(jsonData);

        if (id.length()) {
            if (Firebase.setJSON(firebaseData, this->firebaseRootPath + "/hardware/leds/" + id, FirebaseJson().setJsonData(jsonData)))
                this->sendResponse(HTTP_OK, "success", "Operation was successfully done.");
            else 
                this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
        } else {
            this->sendResponse(HTTP_BAD_REQUEST, "error", "Operation was not successfully done.");
        }
    });
}

void ESP8266_Server::createResponseJSON(char *json, int status, const char *rate, const char *message) {
    const int capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3);
    StaticJsonDocument<capacity> doc;

    doc["status"] = status;
    JsonObject description = doc.createNestedObject("description");
    description["rate"] = rate;
    description["message"] = message;
    serializeJson(doc, json, JSON_BUFF_SIZE);
}

void ESP8266_Server::sendResponse(int status, const char *rate, const char *message) {
    char json[JSON_BUFF_SIZE];
    this->createResponseJSON(json, status, rate, message);

    this->server->send(status, "text/json", json);
}

String ESP8266_Server::setFirebaseRootPath() {
    this->firebaseRootPath += "/" + this->getDashedMacAddress();
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

ESP8266_Hardware ESP8266_Server::getHardware() {
    return this->hardware;
}
