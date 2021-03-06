#ifndef ESP8266_Module_H
#define ESP8266_Module_H

// Include potrebnych kniznic
#include <Arduino.h>
#include "FirebaseJson.h"

// ESP8266_Module
class ESP8266_Module
{
private:

public:
  virtual void init() = 0;
  virtual void run() = 0;
  virtual String toJSON() = 0;
  virtual void parseProperties(String &, String &) = 0;

  template <class MODULE, typename FUN>
  void parseJsonToData(String, String, bool, MODULE *, FUN);

};

// Include template metod
#include "ESP8266_Module.tcc"

#endif // ESP8266_Module_H
