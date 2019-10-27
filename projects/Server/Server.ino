// ESP8266 SERVER LIBRARY
#include <ESP8266_Server.h>

// LED LIBRARY
#include <Led.h>

// LED STATUS
Led strip = Led();
int ledstatus = 0; // status of led (ON/OFF)

// SERVER INITIALIZING
ESP8266WebServer webserver(80);
ESP8266_Server server = ESP8266_Server(&webserver);

const int led = 13;

//void handleNotFound() {
//  digitalWrite(led, 1);
//  String message = "File Not Found\n\n";
//  message += "URI: ";
//  message += server.uri();
//  message += "\nMethod: ";
//  message += (server.method() == HTTP_GET) ? "GET" : "POST";
//  message += "\nArguments: ";
//  message += server.args();
//  message += "\n";
//  for (uint8_t i = 0; i < server.args(); i++) {
//    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
//  }
//  server.send(404, "text/plain", message);
//  digitalWrite(led, 0);
//}

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
  server.handleNotFound();

  // STRIP BEGIN
  strip.init();

//  server.on("/turn-on", HTTP_POST, []() {
//    server.send(200, "text/plain", "Led's were turned on.");
//    
//    // set value
//    if (!Firebase.setInt(firebaseData, "/led/status", 1)) {
//        Serial.print("setting /led/status failed:");
//        Serial.println(firebaseData.errorReason());  
//        return;
//    }
//    ledstatus = 1;
//  });

//  server.on("/turn-off", []() {
//    server.send(200, "text/plain", "Led's were turned off.");
//    
//    // set value
//    if (!Firebase.setInt(firebaseData, "/led/status", 0)) {
//        Serial.print("setting /led failed:");
//        Serial.println(firebaseData.errorReason());  
//        return;
//    }
//    ledstatus = 0;
//  });

//  server.onNotFound(handleNotFound);
}

void loop(void) {
  server.update();

//  if (ledstatus) {
//    strip.on(0, 0, 255, 6, 100);
//  }
//  else {
//    strip.off();
//  }
}
