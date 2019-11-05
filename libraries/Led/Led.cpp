#include "Led.h"
#include "Config.h"

Led::Led() {}

Led::Led(uint16_t numLeds, uint8_t pin) {
    this->setNumLedsOnStrip(numLeds);
    this->setEsp8266Pin(pin);
}

void Led::clear() {
    this->strip.clear();
    this->strip.show();
}

void Led::init() {
    this->strip = Adafruit_NeoPixel(this->numLedsOnStrip, this->esp8266Pin, NEO_GRB + NEO_KHZ800);
    this->strip.begin();
    this->clear();
}

void Led::setEsp8266Pin(uint8_t pin){
    if (pin < MIN_ESP8266_PIN || pin > MAX_ESP8266_PIN)
        this->esp8266Pin = DEFAULT_ESP8266_PIN;
    this->esp8266Pin = pin;
}

void Led::setStatusOfStrip(int status){
    if (status == 0 || status == 1) 
        this->statusOfStrip = status;
    else 
        this->statusOfStrip = 0;
}

void Led::setNumLedsOnStrip(uint16_t numLeds){
    if (numLeds < 1)
        this->numLedsOnStrip = MIN_NUM_OF_LEDS;
    this->numLedsOnStrip = numLeds;
}

void Led::setActiveMode(int mode){
    if (mode != MODE_ZERO && mode != MODE_ONE && mode != MODE_TWO)
        this->activeMode = MODE_ZERO;
    this->activeMode = mode;
}

void Led::setWaitTime(uint8_t ms){
    if (ms < MIN_MS || ms > MAX_MS)
        this->waitTime = 0;
    this->waitTime = ms;
}

void Led::setBrightnessOfStrip(uint8_t brightness){
    if (brightness < MIN_BRIGHTNESS || brightness > MAX_BRIGHTNESS)
        this->brightnessOfStrip = MAX_BRIGHTNESS;
    this->brightnessOfStrip = brightness;
}

void Led::setName(String name){
    this->name = name;
}

String Led::toString() {
    String result = "";

    result += "Name: " + this->name + ", ";
    result += "Status: " + String(this->statusOfStrip) + ", ";
    result += "Mode: " + String(this->activeMode) + ", ";
    result += "Pin: " + String(this->esp8266Pin) + ", ";
    result += "Brightness: " + String(this->brightnessOfStrip) + ", ";
    result += "Wait: " + String(this->waitTime) + ", ";
    result += "NumLeds: " + String(this->numLedsOnStrip);

    return result;
}

