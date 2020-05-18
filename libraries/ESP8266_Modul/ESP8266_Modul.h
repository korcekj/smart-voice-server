#ifndef ESP8266_Modul_H
#define ESP8266_Modul_H

#include <Arduino.h>
#include "FirebaseJson.h"

class ESP8266_Modul
{
private:

public:
  virtual void init() = 0;
  virtual void run() = 0;
  virtual String toJSON() = 0;

  template <class MODULE, typename FUN>
  void parseJsonToData(String, String, bool, MODULE *, FUN);

};

#include "ESP8266_Modul.tcc"

#endif // ESP8266_Modul_H
