#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class Led {
private:
    Adafruit_NeoPixel strip;

    void clear();

public:
    Led();

    void init();
    void on(uint8_t, uint8_t, uint8_t, uint16_t, uint8_t);
    void off();
};

#endif // LED_H
