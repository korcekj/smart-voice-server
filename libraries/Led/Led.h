#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <vector>

class Led {
private:
    Adafruit_NeoPixel strip;
    uint8_t esp8266Pin;
    int statusOfStrip;
    uint16_t numLedsOnStrip;
    int activeMode;
    uint8_t waitTime;
    uint8_t brightnessOfStrip;
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
    void setName(String);

    String toString();
    
};

#endif // LED_H
