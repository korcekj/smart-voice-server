#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Config.h"
#include <vector>

class Led {
private:
    Adafruit_NeoPixel strip;
    uint8_t esp8266Pin;
    int statusOfStrip = L_OFF;
    uint16_t numLedsOnStrip;
    int activeMode = MODE_ZERO;
    uint8_t waitTime = MIN_MS;
    uint8_t brightnessOfStrip = MAX_BRIGHTNESS;
    std::vector<std::vector<uint8_t>> colors;
    String name;

    void clear();

public:
    Led();
    Led(uint16_t, uint8_t);

    void init();
    void setEsp8266Pin(uint8_t);
    void setStatusOfStrip(int);
    void setNumLedsOnStrip(uint16_t);
    void setActiveMode(int);
    void setWaitTime(uint8_t);
    void setBrightnessOfStrip(uint8_t);
    void setColorBytes(uint8_t, uint8_t, uint8_t);
    void setName(String);

    String toString();
    
};

#endif // LED_H
