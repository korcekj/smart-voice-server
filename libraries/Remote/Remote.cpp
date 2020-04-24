#include "Remote.h"

const int SONY_COMMANDS[] = {SONY_POWER, SONY_VOLUME_UP, SONY_VOLUME_DOWN, SONY_CHANNEL_UP, SONY_CHANNEL_DOWN};

Remote::Remote() {}

void Remote::init(IRsend *irSend) {
    if (irSend == nullptr) return;

    this->irSend = irSend;
    this->irSend->begin();
}

void Remote::run() {
    if (this->irSend == nullptr) return;

    if (this->command != -1) {
        (this->*this->typeFunctions[this->type])();
        this->setCommand(-1);
    }
}

void Remote::Sony() {
    if (this->command < 0 || this->command > SONY_NUM_COMM - 1) return;
    this->irSend->sendSony(SONY_COMMANDS[this->command], kSony12Bits, kSonyMinRepeat);
}

void Remote::setEsp8266Pin(uint8_t pin){
    if (pin < MIN_ESP8266_PIN || pin > MAX_ESP8266_PIN)
        return;
    this->esp8266Pin = pin;
}

void Remote::setName(String name) {
    this->name = name;
}

void Remote::setType(uint8_t type) {
    if (type < MIN_NUM_TYPE || type > MAX_NUM_TYPE)
        return;
    this->type = type;
}

void Remote::setFrequency(uint8_t frequency) {
    this->frequency = frequency;
}

void Remote::setCommand(uint8_t command) {
    this->command = command;
}

uint8_t Remote::getEsp8266Pin() {
    return this->esp8266Pin;
}

String Remote::toJSON() {
    String result = "{";

    result += "\"pin\":" + String(this->esp8266Pin) + ",";
    result += "\"name\":\"" + this->name + "\",";
    result += "\"type\":" + String(this->type) + ",";
    result += "\"frequency\":" + String(this->frequency);

    result += "}";

    return result;
}

