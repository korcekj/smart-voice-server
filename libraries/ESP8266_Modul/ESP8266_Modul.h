#ifndef ESP8266_Modul_H
#define ESP8266_Modul_H

#include <Arduino.h>

class ESP8266_Modul
{
private:

public:
  virtual void init(void *) = 0;
  virtual void run() = 0;
  virtual String toJSON() = 0;

};

#endif // ESP8266_Modul_H
