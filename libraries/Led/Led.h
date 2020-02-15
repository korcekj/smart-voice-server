#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Config.h"
#include <vector>
#include <map>

class Led {
private:
    Adafruit_NeoPixel *strip = nullptr;

    uint8_t esp8266Pin = DEFAULT_ESP8266_PIN;
    int statusOfStrip = L_OFF;
    uint16_t numLedsOnStrip = MIN_NUM_OF_LEDS;
    int activeMode = MODE_ZERO;
    uint8_t waitTime = MIN_MS;
    uint8_t brightnessOfStrip = MAX_BRIGHTNESS;
    std::vector<std::map<String, uint8_t>> colors = {{ {"r", 255}, {"g", 255}, {"b", 255} }};
    String name = "";

    void clear();

public:
    Led();
    Led(uint16_t, uint8_t);

    void init(Adafruit_NeoPixel *);
    void run();
    void clearColors();

    void setEsp8266Pin(uint8_t);
    void setStatusOfStrip(int);
    void setNumLedsOnStrip(uint16_t);
    void setActiveMode(int);
    void setWaitTime(uint8_t);
    void setBrightnessOfStrip(uint8_t);
    void setColorBytes(uint8_t, uint8_t, uint8_t);
    void setName(String);

    uint8_t getEsp8266Pin();

    String toJSON();

};

#endif // LED_H
