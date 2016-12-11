#ifndef IR_CAMERA_H
#define IR_CAMERA_H

#include "Arduino.h"

class IRCamera {
public:
  static IRCamera* getInstance() {
    static IRCamera instance;
    return &instance;
  }
  struct IRTarget {
    int xPos;
    int yPos;
    int size;
  };
  IRTarget getTarget(); //returns the flame target or -1,-1 if no valid targets
  void init(); //sets up camera
private:
  IRCamera();
  void readTargets(); //updates the four targets below
  IRTarget targets[4];
  void writeTwoIICByte(uint8_t first, uint8_t second);
  static const byte ADDRESS = 0xB0>>1;
};

#endif
