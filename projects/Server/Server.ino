// ESP8266_Server kniznica
#include <ESP8266_Server.h>

// ESP8266_Hardware kniznica
#include <ESP8266_Hardware.h>


// Vytvorenie instancie ESP8266WebServer s portom: `80`
ESP8266WebServer webserver(80);

// Vytvorenie instancii hardveru a servera
ESP8266_Hardware hardware = ESP8266_Hardware();
ESP8266_Server server = ESP8266_Server(&webserver, &hardware);

// Led pin na ESP8266 Wemos D1 Wi-Fi module
const int led = 13;

// Nastavenie programu
void setup(void) {
  // Vypnutie Led na ESP8266 module
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  // Nastavenie monitor serialu na baud rate: `115200` b/s
  Serial.begin(115200);

  // Inicializacia servera
  server.init();

  // Kontrolny vypis servera a jeho nastaveni
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(server.getIpAddress());
  Serial.print("MAC Address: ");
  Serial.println(server.getMacAddress());
  Serial.print("URL: ");
  Serial.println(server.getUrl());

  // Zavolanie metod objektu server na handlovanie HTTP requestov
  server.handleRoot();
  server.handleLed();
  server.handleRemote();
  server.handleNotFound();
}

// Beh programu
void loop(void) {
  // Aktualizacia servera
  server.update();
  // Aktualizacia hardwarovych sucasti
  hardware.update();
}
