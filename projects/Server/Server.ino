// ESP8266 SERVER LIBRARY
#include <ESP8266_Server.h>

// ESP8266 HARDWARE LIBRARY
#include <ESP8266_Hardware.h>


// SERVER AND HARDWARE SET UP
ESP8266WebServer webserver(80);
ESP8266_Hardware hardware = ESP8266_Hardware();
ESP8266_Server server = ESP8266_Server(&webserver, &hardware);

// HARDWARE PARTS
std::vector<Adafruit_NeoPixel> ledStrips{ 
  Adafruit_NeoPixel(), 
  Adafruit_NeoPixel(),
  Adafruit_NeoPixel(), 
  Adafruit_NeoPixel(), 
  Adafruit_NeoPixel(), 
  Adafruit_NeoPixel(), 
  Adafruit_NeoPixel(), 
  Adafruit_NeoPixel(), 
  Adafruit_NeoPixel(), 
  Adafruit_NeoPixel(), 
  Adafruit_NeoPixel() 
}; 

std::vector<IRsend> irSends{ 
  IRsend(0), 
  IRsend(1),
  IRsend(2), 
  IRsend(3), 
  IRsend(4), 
  IRsend(5), 
  IRsend(12), 
  IRsend(13), 
  IRsend(14), 
  IRsend(15), 
  IRsend(16) 
}; 

const int led = 13;

// SETUP
void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);

  // HARDWARE INIT
  createStrips();
  createIRsends();

  // SERVER INIT
  server.init();
  
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(server.getIpAddress());
  Serial.print("MAC Address: ");
  Serial.println(server.getMacAddress());
  Serial.print("URL: ");
  Serial.println(server.getUrl());
  Serial.print("LED: ");
  Serial.println(hardware.getLeds());
  Serial.print("REMOTE: ");
  Serial.println(hardware.getRemotes());
  
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

// Create strips for hardware library and manage them in memory
void createStrips() {
    for (auto &ledStrip : ledStrips)
    {
        hardware.addStrip(&ledStrip);
    }
}

// Create IRsend for hardware library and manage them in memory
void createIRsends() {
    for (auto &irSend : irSends)
    {
        hardware.addIRsend(&irSend);
    }
}
