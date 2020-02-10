// ESP8266 SERVER LIBRARY
#include <ESP8266_Server.h>

// SERVER INITIALIZING
ESP8266WebServer webserver(80);
ESP8266_Server server = ESP8266_Server(&webserver);

const int led = 13;

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);

  // SERVER BEGIN
  server.init();
  
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(server.getIpAddress());
  Serial.print("MAC Address: ");
  Serial.println(server.getMacAddress());
  Serial.print("URL: ");
  Serial.println(server.getUrl());
  
  server.handleRoot();
  server.handleLed();
  server.handleNotFound();

  // STRIP BEGIN
  strip.init();
}

void loop(void) {
  server.update();
}
