#ifndef ESP8266_SERVER_H
#define ESP8266_SERVER_H

#include <Arduino.h>
#include "ESP8266_Hardware.h"
#include <ESP8266WebServer.h>

class ESP8266_Server 
{
private:
    ESP8266WebServer *server;
    ESP8266_Hardware hardware;
    IPAddress ip;
    String mac;
    String url;
    bool dnsRunning;
    String firebaseRootPath;

    void serverInit();
    void firebaseInit();
    void hardwareInit();
    void hardwareLedsInit();
    void connect();
    void runDns();
    void createResponseJSON(char *, int, const char *, const char *);
    void sendResponse(int, const char *, const char *);
    String getDashedMacAddress();
    String setFirebaseRootPath();

public:
    ESP8266_Server(ESP8266WebServer *);

    void init();
    void update();
    void handleRoot(String = "/", HTTPMethod = HTTP_GET);
    void handleNotFound();

    String getIpAddress();
    String getMacAddress();
    String getUrl();
    ESP8266_Hardware getHardware();

};

#endif // ESP8266_SERVER_H
