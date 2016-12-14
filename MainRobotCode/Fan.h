#ifndef FAN_H
#define FAN_H
#include "Arduino.h"
#include "Servo.h"
class Fan {
public:
  static Fan* getInstance() {
    static Fan instance;
    return &instance;
  }
  void init();
  void setPwr(unsigned int pwr); //0 to 100
private:
  Servo myServo;
  Fan();
};

#endif
