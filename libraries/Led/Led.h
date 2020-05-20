#ifndef LED_H 
#define LED_H

// Include potrebnych kniznic
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "ESP8266_Modul.h"
#include "Config.h"
#include <vector>
#include <map>

// Led dediaca od ESP8266_Modul
class Led : public ESP8266_Modul {
private:
    Adafruit_NeoPixel *strip = nullptr;

    uint8_t esp8266Pin = DEFAULT_ESP8266_PIN;
    uint8_t statusOfStrip = L_OFF;
    uint16_t numLedsOnStrip = MIN_NUM_OF_LEDS;
    uint8_t activeMode = MODE_ZERO;
    int waitTime = MIN_MS;
    uint8_t brightnessOfStrip = MAX_BRIGHTNESS;
    std::vector<std::map<String, uint8_t>> colors = {{ {"r", 255}, {"g", 255}, {"b", 255}, {"a", 100} }};
    String name = "";

    std::vector<void (Led::*)()> modeFunctions = {&Led::mode0, &Led::mode1, &Led::mode2};
    unsigned long timeNow = millis();
    uint16_t currentIndex = 0;

    void clear();
    void mode0();
    void mode1();
    void mode2();
    uint32_t wheel(byte);

public:
    Led();
    ~Led();

    void init();
    void run();
    String toJSON();
    
    void parseProperties(String &, String &);
    void parseColorBytes(String &, String &);
    void clearColors();

    void setEsp8266Pin(uint8_t);
    void setStatusOfStrip(uint8_t);
    void setNumLedsOnStrip(uint16_t);
    void setActiveMode(uint8_t);
    void setWaitTime(int);
    void setBrightnessOfStrip(uint8_t);
    void setColorBytes(uint8_t, uint8_t, uint8_t, uint8_t);
    void setName(String);

    uint8_t getEsp8266Pin();
    std::map<String, uint8_t> getColorBytes(uint8_t);

};

#endif // LED_H
