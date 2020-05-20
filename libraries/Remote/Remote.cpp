#include "Remote.h"

// Definovanie SONY prikazov 
const int SONY_COMMANDS[] = {SONY_POWER, SONY_VOLUME_UP, SONY_VOLUME_DOWN, SONY_CHANNEL_UP, SONY_CHANNEL_DOWN};

// Konstruktor
Remote::Remote() {}

// Destruktor
Remote::~Remote() { 
    delete this->irSend; 
}

// Metoda sluziacia na inicializaciu IR vysielaca
void Remote::init() {
    this->irSend = new IRsend(this->esp8266Pin);
    this->irSend->begin();
}

// Metoda sluziaca na beh vysielaca v case
void Remote::run() {
    if (this->irSend == nullptr) return;
    // Vykonanie prikazu len v pripade ak bol nastaveny
    if (this->command != -1) {
        // Vykonanie prikazu
        (this->*this->typeFunctions[this->type])();
        // Reset prikazu
        this->setCommand(-1);
    }
}

// Metoda sluziaca na vykonanie SONY prikazu
void Remote::Sony() {
    // Validacia indexu prikazu
    if (this->command < 0 || this->command > SONY_NUM_COMM - 1) return;
    // Vykonanie prikazu zaslanim zadefinovanych hodnot IR vysielacom
    this->irSend->sendSony(SONY_COMMANDS[this->command], kSony12Bits, kSonyMinRepeat);
}

// Metoda sluziaca na nastavenie prislusneho pinu na ESP8266 module
void Remote::setEsp8266Pin(uint8_t pin){
    if (pin < MIN_ESP8266_PIN || pin > MAX_ESP8266_PIN)
        return;
    this->esp8266Pin = pin;
}

// Metoda sluziaca na nastavenie mena ovladaca
void Remote::setName(String name) {
    this->name = name;
}

// Metoda na nastavenie typu ovladaca
void Remote::setType(uint8_t type) {
    if (type < MIN_NUM_TYPE || type > MAX_NUM_TYPE)
        return;
    this->type = type;
}

// Metoda na nastavenie frekvenie ovladaca
void Remote::setFrequency(uint8_t frequency) {
    this->frequency = frequency;
}

// Metoda na nastavenie prikazu pre ovladac
void Remote::setCommand(uint8_t command) {
    this->command = command;
}

// Metoda na ziskanie aktualne pripojeneho pinu
uint8_t Remote::getEsp8266Pin() {
    return this->esp8266Pin;
}

// Metoda sluziaca na parsovanie dat ovladaca a nasledne priradenie dat k premennym
void Remote::parseProperties(String &key, String &value) {
    if (key == R_PIN)
        this->setEsp8266Pin(value.toInt());
    else if (key == R_NAME)
        this->setName(value);
    else if (key == R_TYPE)
        this->setType(value.toInt());
    else if (key == R_FREQUENCY)
        this->setFrequency(value.toInt());
    else if (key == R_COMMAND)
        this->setCommand(value.toInt());
}

// Metoda sluziaca na vypis parametrov v JSON formate
String Remote::toJSON() {
    String result = "{";

    result += "\"pin\":" + String(this->esp8266Pin) + ",";
    result += "\"name\":\"" + this->name + "\",";
    result += "\"type\":" + String(this->type) + ",";
    result += "\"frequency\":" + String(this->frequency);

    result += "}";

    return result;
}

