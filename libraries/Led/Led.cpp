#include "Led.h"

Led::Led() {}

Led::Led(uint16_t numLeds, uint8_t pin) {
    this->setNumLedsOnStrip(numLeds);
    this->setEsp8266Pin(pin);
}

void Led::clear() {
    this->strip->clear();
    this->strip->show();
}

void Led::init(Adafruit_NeoPixel *strip) {
    if (strip == nullptr) return;

    this->strip = strip;
    this->strip->updateType(NEO_GRB + NEO_KHZ800);
    this->strip->updateLength(this->numLedsOnStrip);
    this->strip->setPin(this->esp8266Pin);
    this->strip->begin();
    this->clear();
}

void Led::run() {
    if (this->strip == nullptr) return;

    if (this->statusOfStrip == L_OFF) this->clear();
    else {
        for(uint16_t i = 0; i < strip->numPixels(); i++) {
            strip->setPixelColor(i, strip->Color(0, 0, 255));
            strip->show();
            delay(this->waitTime);
        }
    }
}

void Led::clearColors() {
    this->colors.clear();
}

void Led::setEsp8266Pin(uint8_t pin){
    if (pin < MIN_ESP8266_PIN || pin > MAX_ESP8266_PIN)
        return;
    this->esp8266Pin = pin;
}

void Led::setStatusOfStrip(int status){
    if (status == L_OFF || status == L_ON) 
        this->statusOfStrip = status;
}

void Led::setNumLedsOnStrip(uint16_t numLeds){
    if (numLeds < 1)
        return;
    this->numLedsOnStrip = numLeds;
}

void Led::setActiveMode(int mode){
    if (mode != MODE_ZERO && mode != MODE_ONE && mode != MODE_TWO)
        return;
    this->activeMode = mode;
}

void Led::setWaitTime(uint8_t ms){
    if (ms < MIN_MS || ms > MAX_MS)
        return;
    this->waitTime = ms;
}

void Led::setBrightnessOfStrip(uint8_t brightness){
    if (brightness < MIN_BRIGHTNESS || brightness > MAX_BRIGHTNESS)
        return;
    this->brightnessOfStrip = brightness;
}

void Led::setColorBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    std::map<String, uint8_t> color;
    color.insert(std::pair<String, uint8_t>("r", r));
    color.insert(std::pair<String, uint8_t>("g", g));
    color.insert(std::pair<String, uint8_t>("b", b));
    color.insert(std::pair<String, uint8_t>("a", a));
    this->colors.push_back(color);
}

void Led::setName(String name){
    this->name = name;
}

uint8_t Led::getEsp8266Pin() {
    return this->esp8266Pin;
}

String Led::toJSON() {
    String result = "{";

    result += "\"name\":\"" + this->name + "\",";
    result += "\"status\":" + String(this->statusOfStrip) + ",";
    result += "\"mode\":" + String(this->activeMode) + ",";
    result += "\"pin\":" + String(this->esp8266Pin) + ",";
    result += "\"brightness\":" + String(this->brightnessOfStrip) + ",";
    result += "\"wait\":" + String(this->waitTime) + ",";
    result += "\"numLeds\":" + String(this->numLedsOnStrip) + ",";
    result += "\"colors\":{";

    for(int i = 0; i < this->colors.size(); i++) {
        result += "\"color" + String(i) + "\":{";
        int counter = 0;
        for (auto &byte : this->colors[i])
        {
            result += "\"" + byte.first + "\":";

            if (byte.first == "a") result += String((double)byte.second / 100);
            else result += String(byte.second);

            if (++counter < 4) result += ",";
        }
        result += "}";
        if (i < this->colors.size() - 1) result += ",";
    }
    result += "}}";

    return result;
}

