// ESP8266 SERVER LIBRARY
#include <ESP8266_Server.h>

// ESP8266 HARDWARE LIBRARY
#include <ESP8266_Hardware.h>


// SERVER AND HARDWARE SET UP
ESP8266WebServer webserver(80);
ESP8266_Hardware hardware = ESP8266_Hardware();
ESP8266_Server server = ESP8266_Server(&webserver, &hardware);

const int led = 13;

// SETUP
void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);

  // SERVER INIT
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
  server.handleRemote();
  server.handleNotFound();
}

// LOOP
void loop(void) {
  server.update();
  hardware.runHardware();
}
