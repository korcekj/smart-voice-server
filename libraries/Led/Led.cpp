#include "Led.h"

Led::Led() {}

Led::Led(uint16_t numLeds, uint8_t pin) {
    this->setNumLedsOnStrip(numLeds);
    this->setEsp8266Pin(pin);
}

void Led::clear() {
    if (this->strip == nullptr) return;

    this->strip->clear();
    this->strip->show();
    this->currentIndex = 0;
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
    else if (this->statusOfStrip == L_OFF) {
        this->clear();
        return;
    }

    (this->*this->modeFunctions[this->activeMode])();
}

void Led::mode0() {
    std::map<String, uint8_t> color = this->getColorBytes(0);
    for(uint16_t i = 0; i < this->strip->numPixels(); i++) {
        this->strip->setPixelColor(i, this->strip->Color(color["r"], color["g"], color["b"], color["a"])); 
        this->strip->setBrightness(this->brightnessOfStrip);
        this->strip->show();
    }
}

void Led::mode1() {
    if (millis() >= this->timeNow + this->waitTime) {
        if (this->currentIndex >= this->strip->numPixels() + 3) this->clear();

        std::map<String, uint8_t> color = this->getColorBytes(this->currentIndex % 3);
        strip->setPixelColor(this->currentIndex, this->strip->Color(color["r"], color["g"], color["b"], color["a"]));
        strip->setPixelColor(this->currentIndex - 3, 0); 
        this->strip->setBrightness(this->brightnessOfStrip);
        this->strip->show();

        this->currentIndex++;
        this->timeNow += this->waitTime;
    }
}

void Led::mode2() {
    if (millis() >= this->timeNow + this->waitTime) {
        if (this->currentIndex >= 256) this->currentIndex = 0;

        for(uint16_t i = 0; i < this->strip->numPixels(); i++) {
            this->strip->setPixelColor(i, this->wheel((i + this->currentIndex) & 255));
        }

        this->strip->setBrightness(this->brightnessOfStrip);
        this->strip->show();
        
        this->currentIndex++;
        this->timeNow += this->waitTime;
    }
}

uint32_t Led::wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }

  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }

  WheelPos -= 170;
  return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void Led::clearColors() {
    this->colors.clear();
}

void Led::setEsp8266Pin(uint8_t pin){
    if (pin < MIN_ESP8266_PIN || pin > MAX_ESP8266_PIN)
        return;
    this->esp8266Pin = pin;
}

void Led::setStatusOfStrip(uint8_t status){
    if (status == L_OFF) {
        this->statusOfStrip = L_OFF;
    } else if (status == L_ON) {
        this->clear();
        this->timeNow = millis();
        this->statusOfStrip = L_ON;
    }
}

void Led::setNumLedsOnStrip(uint16_t numLeds){
    if (numLeds < 1)
        return;
    this->numLedsOnStrip = numLeds;
}

void Led::setActiveMode(uint8_t mode){
    if (mode != MODE_ZERO && mode != MODE_ONE && mode != MODE_TWO)
        return;

    if (this->activeMode != mode) {
        this->clear();
        this->timeNow = millis();
    }

    this->activeMode = mode;
}

void Led::setWaitTime(int ms){
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

std::map<String, uint8_t> Led::getColorBytes(uint8_t index) {
    if (index < this->colors.size()) {
        return this->colors[index];
    }

    return this->colors[0];
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

