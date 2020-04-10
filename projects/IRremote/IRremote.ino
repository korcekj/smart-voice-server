/*
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * This is very simple teaching code to show you how to use the library.
 * If you are trying to decode your Infra-Red remote(s) for later replay,
 * use the IRrecvDumpV2.ino example code instead of this.
 * An IR detector/demodulator must be connected to the input kRecvPin.
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 * Example circuit diagram:
 *  https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.2 June, 2017
 *     Changed GPIO pin to the same as other examples.
 *     Used our own method for printing a uint64_t.
 *     Changed the baud rate to 115200.
 *   Version 0.1 Sept, 2015
 *     Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// An IR detector/demodulator is connected to GPIO pin 14(D5 on a NodeMCU
// board).
// Note: GPIO 16 won't work on the ESP8266 as it does not have interrupts.
const uint16_t kRecvPin = 2;

IRrecv irrecv(kRecvPin);

decode_results results;

void setup() {
  Serial.begin(115200);
  irrecv.enableIRIn();  // Start the receiver
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);
  Serial.println();
  Serial.print("IRrecvDemo is now running and waiting for IR message on Pin ");
  Serial.println(kRecvPin);
}

void loop() {
  if (irrecv.decode(&results)) {
    
    // print() & println() can't handle printing long longs. (uint64_t)
    
    // SetTopBox Lion 2
    if (results.value == 0xFB001BE4) {
      Serial.println("Volume Up");
    } else if (results.value == 0xFB00DB24) {
      Serial.println("Volume Down");
    } else if (results.value == 0xFB007B84) {
      Serial.println("Canal Up");
    } else if (results.value == 0xFB006B94) {
      Serial.println("Canal Down");
    } else if (results.value == 0xFB003BC4) {
      Serial.println("Power On/Off");
    } 

    // TV Sony Bravia
    if (results.value == 0xF124B0B6) {
      Serial.println("Power On/Off");
    } else if (results.value == 0xB44D8DFB) {
      Serial.println("Volume Up");
    } else if (results.value == 0x43898CC0) {
      Serial.println("Volume Down");
    } else if (results.value == 0xBC392255) {
      Serial.println("Canal Up");
    } else if (results.value == 0xFFAFB8DC) {
      Serial.println("Canal Down");
    } else {
      Serial.print("Value: ");
      serialPrintUint64(results.value, HEX);
      Serial.print("\nCommand: ");
      serialPrintUint64(results.command, HEX);
      Serial.println("\n---------------------------------");
    }
    
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}
