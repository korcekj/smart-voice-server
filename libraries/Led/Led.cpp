#include "Led.h"

// Konstruktor
Led::Led() {}

// Destruktor
Led::~Led() { 
    delete this->strip; 
}

// Metoda na "vycistenie" stavu LED - nastavenie vsetkych pixelov na: `0`
void Led::clear() {
    if (this->strip == nullptr) return;

    this->strip->clear();
    this->strip->show();
    this->currentIndex = 0;
}

// Metoda sluziaca na inicializaciu Adafruit_NeoPixel
void Led::init() {
    this->strip = new Adafruit_NeoPixel();
    this->strip->updateType(NEO_GRB + NEO_KHZ800);
    this->strip->updateLength(this->numLedsOnStrip);
    this->strip->setPin(this->esp8266Pin);
    this->strip->begin();
    this->clear();
}

// Metoda sluziaca na beh LED v case
void Led::run() {
    if (this->strip == nullptr) return;
    // V pripade ze je LED vypnuta, "vycisti" LED
    else if (this->statusOfStrip == L_OFF) {
        this->clear();
        return;
    }
    // Beh modu LED len v pripade, ak je v stave: `L_ON`
    (this->*this->modeFunctions[this->activeMode])();
}

// Metoda sluziaca na spustenie modu 0 - zobrazenie 1 farby
void Led::mode0() {
    // Ziskanie 1. farby
    std::map<String, uint8_t> color = this->getColorBytes(0);
    
    // Nastavnie kazdeho pixelu na prislusnu farbu
    for(uint16_t i = 0; i < this->strip->numPixels(); i++) {
        this->strip->setPixelColor(i, this->strip->Color(color["r"], color["g"], color["b"], color["a"])); 
        this->strip->setBrightness(this->brightnessOfStrip);
        this->strip->show();
    }
}

// Metoda sluziaca na spustenie modu 1 - zobrazenie 3 farieb
void Led::mode1() {
    // V pripade ak cas na spustenie operacie je >= ako cas od zaciatku behu programu, vykonaj operacie nizsie
    if (millis() >= this->timeNow + this->waitTime) {
        // V pripade vyfarbenia vsetkych pixelov (krok 3 pixely), "vycisti" LED
        if (this->currentIndex >= this->strip->numPixels() + 3) this->clear();

        // Ziskanie farby na zaklade aktualneho indexu
        std::map<String, uint8_t> color = this->getColorBytes(this->currentIndex % 3);
        strip->setPixelColor(this->currentIndex, this->strip->Color(color["r"], color["g"], color["b"], color["a"]));
        strip->setPixelColor(this->currentIndex - 3, 0); 
        this->strip->setBrightness(this->brightnessOfStrip);
        this->strip->show();
        
        this->currentIndex++;
        this->timeNow += this->waitTime;
    }
}

// Metoda sluziaca na spustenie modu 1 - zobrazenie duhy farieb
void Led::mode2() {
    // V pripade ak cas na spustenie operacie je >= ako cas od zaciatku behu programu, vykonaj operacie nizsie
    if (millis() >= this->timeNow + this->waitTime) {
        // V pripade vyfarbenia vsetkych pixelov, "vycisti" LED
        if (this->currentIndex >= 256) this->currentIndex = 0;

        // Nastavenie kazdeho pixelu farbou na zaklade vypoctu jej hodnoty metodou: `wheel`
        for(uint16_t i = 0; i < this->strip->numPixels(); i++) {
            this->strip->setPixelColor(i, this->wheel((i + this->currentIndex) & 255));
        }

        this->strip->setBrightness(this->brightnessOfStrip);
        this->strip->show();
        
        this->currentIndex++;
        this->timeNow += this->waitTime;
    }
}

// Metoda na vypocet hodnoty farby v RGB 
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

// Metoda sluziaca na parsovanie dat LED a nasledne priradenie dat k premennym
void Led::parseProperties(String &key, String &value) {
    if (key == P_BRIGHTNESS)
        this->setBrightnessOfStrip(value.toInt());
    else if (key == P_MODE)
        this->setActiveMode(value.toInt());
    else if (key == P_NAME)
        this->setName(value);
    else if (key == P_NUM_LEDS)
        this->setNumLedsOnStrip(value.toInt());
    else if (key == P_PIN)
        this->setEsp8266Pin(value.toInt());
    else if (key == P_STATUS)
        this->setStatusOfStrip(value.toInt());
    else if (key == P_WAIT)
        this->setWaitTime(value.toInt());
    else if (key == P_COLORS) {
        // Resetovanie farieb
        this->clearColors();
        // Znovu parsovanie poskytnutych farieb v JSON formate
        ESP8266_Module::parseJsonToData(value, P_COLOR, true, this, &Led::parseColorBytes);
    }
}

// Metoda sluziaca na parosvanie RGB hodnot farieb
void Led::parseColorBytes(String &key, String &value) {
    // FirebaseJson objekt
    FirebaseJson json;
    // RGBa predvolene hodnoty
    uint8_t r, g, b, a = 100;

    // Nastavenie json dat
    json.setJsonData(value);
    // Preparsovanie RGBa dat a nasledne priradenie premennym
    r = json.parse().get("r").parseResult().intValue;
    g = json.parse().get("g").parseResult().intValue;
    b = json.parse().get("b").parseResult().intValue;

    if (json.parse().get("a").parseResult().success) {
        String brightness = json.parse().get("a").parseResult().stringValue;
        a = brightness.toDouble() * 100;
    }
    
    // Nastavenie parsovanych hodnot premennym LED
    this->setColorBytes(r, g, b, a);
}

// Metoda na resetovanie farieb LED
void Led::clearColors() {
    this->colors.clear();
}

// Metoda na nastavenie prislusneho pinu na ESP8266 Wemos D1 mini
void Led::setEsp8266Pin(uint8_t pin){
    if (pin < MIN_ESP8266_PIN || pin > MAX_ESP8266_PIN)
        return;
    this->esp8266Pin = pin;
}

// Metoda na nastvenie statusu LED
void Led::setStatusOfStrip(uint8_t status){
    if (status == L_OFF) {
        this->statusOfStrip = L_OFF;
    } else if (status == L_ON) {
        this->clear();
        this->timeNow = millis();
        this->statusOfStrip = L_ON;
    }
}

// Metoda na nastavenie poctu LED na pasiku
void Led::setNumLedsOnStrip(uint16_t numLeds){
    if (numLeds < 1)
        return;
    this->numLedsOnStrip = numLeds;
}

// Metoda na nastavenie aktualneho modu
void Led::setActiveMode(uint8_t mode){
    if (mode != MODE_ZERO && mode != MODE_ONE && mode != MODE_TWO)
        return;

    if (this->activeMode != mode) {
        this->clear();
        this->timeNow = millis();
    }

    this->activeMode = mode;
}

// Metoda na nastavenie casu cakania na zmenu
void Led::setWaitTime(int ms){
    if (ms < MIN_MS || ms > MAX_MS)
        return;
    this->waitTime = ms;
}

// Metoda na nastavenie jasu LED
void Led::setBrightnessOfStrip(uint8_t brightness){
    if (brightness < MIN_BRIGHTNESS || brightness > MAX_BRIGHTNESS)
        return;
    this->brightnessOfStrip = brightness;
}

// Metoda na nastavenie jednotlivych RGBa hodnot 
void Led::setColorBytes(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    std::map<String, uint8_t> color;
    color.insert(std::pair<String, uint8_t>("r", r));
    color.insert(std::pair<String, uint8_t>("g", g));
    color.insert(std::pair<String, uint8_t>("b", b));
    color.insert(std::pair<String, uint8_t>("a", a));
    this->colors.push_back(color);
}

// Metoda na nastvavenie mena LED
void Led::setName(String name){
    this->name = name;
}

// Metoda na ziskanie aktualne pripojeneho pinu
uint8_t Led::getEsp8266Pin() {
    return this->esp8266Pin;
}

// Metoda na ziskanie RGBa hodnot na zaklade poskytnuteho indexu: `index`
std::map<String, uint8_t> Led::getColorBytes(uint8_t index) {
    if (index < this->colors.size()) {
        return this->colors[index];
    }

    return this->colors[0];
}

// Metoda sluziaca na vypis parametrov v JSON formate
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

