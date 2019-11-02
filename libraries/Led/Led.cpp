#include "Led.h"
#include "Config.h"

Led::Led() {}

Led::Led(uint16_t numLeds, uint8_t pin) {
    this->numLedsOnStrip = numLeds;
    this->esp8266Pin = pin;
}

void Led::clear() {
    this->strip.clear();
    this->strip.show();
}

void Led::init() {
    if (this->esp8266Pin == NULL || this->numLedsOnStrip == NULL) 
        return;
    this->strip = Adafruit_NeoPixel(this->numLedsOnStrip, this->esp8266Pin, NEO_GRB + NEO_KHZ800);
    this->strip.begin();
    this->clear();
}

void Led::setEsp8266Pin(uint8_t pin){
    if (pin < 0 || pin > 16)
        return;
    
    this->esp8266Pin = pin;
}

void Led::setStatusOfStrip(int status){
    if (status == 0 || status == 1) {
        this->statusOfStrip = status;
        return;
    }
}

void Led::setNumLedsOnStrip(uint16_t numLeds){
    if (numLeds < 1)
        return;
    this->numLedsOnStrip = numLeds;
}

void Led::setActiveMode(int mode){
    if (mode != MODE_ZERO || mode != MODE_ONE || mode != MODE_TWO)
        return;
    this->activeMode = mode;
}

void Led::setWaitTime(uint8_t wait){
    if (wait < 0)
        return;
    this->waitTime = wait;
}

void Led::setBrightnessOfStrip(uint8_t brightness){
    if (brightness < 0 || brightness > 100)
        return;
    this->brightnessOfStrip = brightness;
}

void Led::setName(String name){
    this->name = name;
}

String Led::getName() {
    return this->name;
}

