#include "Fan.h"
#include "Pinout.h"

Fan::Fan():myServo() {
}

void Fan::init() {
  myServo.attach(FAN_PIN);
  myServo.write(0);
  delay(1000);
}

void Fan::setPwr(unsigned int newPwr) {
  myServo.write(map(newPwr,0,100,0,180));
}
