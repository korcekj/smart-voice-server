#ifndef ESP8266_SERVER_H
#define ESP8266_SERVER_H

// Include potrebnych kniznic
#include <Arduino.h> 
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include "ESP8266_Hardware.h"
#include <ESP8266WebServer.h>
#include "FirebaseESP8266.h"
#include "Config.h"
#include <vector>

// ESP8266_Server
class ESP8266_Server 
{
private:
    ESP8266WebServer *server;
    ESP8266_Hardware *hardware;
    IPAddress ip;
    String mac;
    String url;
    bool dnsRunning;
    String firebaseRootPath;
    String firebaseError;

    void serverInit();
    void firebaseInit();
    void hardwareInit();
    void hardwareInitByType(String, bool (ESP8266_Hardware::*)(String, String, bool));

    void connect();
    void runDns();

    void createResponseJSON(char *, int, const char *, const char *, const char *);
    void sendResponse(int, const char *, const char *, const char * = "");

    bool isUserOwner(const String &, const String &);
    bool containArgs(std::vector<String> &);

    void setFirebaseRootPath();

    String getDashedMacAddress();

    // LED url
    void handleCreateLed(String, HTTPMethod = HTTP_POST);
    void handleUpdateLed(String, HTTPMethod = HTTP_POST);
    void handleDeleteLed(String, HTTPMethod = HTTP_POST);

    // REMOTE url
    void handleCreateRemote(String, HTTPMethod = HTTP_POST);
    void handleUpdateRemote(String, HTTPMethod = HTTP_POST);
    void handleDeleteRemote(String, HTTPMethod = HTTP_POST);

public:
    ESP8266_Server(ESP8266WebServer *, ESP8266_Hardware *);

    void init();
    void update();
    void handleRoot(String = "/", HTTPMethod = HTTP_GET);
    void handleNotFound();
    void handleLed();
    void handleRemote();

    void setIpAddress(IPAddress);
    void setMacAddress(String);
    String getIpAddress();
    String getMacAddress();
    String getUrl();
};

#endif // ESP8266_SERVER_H
