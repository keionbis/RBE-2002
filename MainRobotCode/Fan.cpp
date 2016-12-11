#include "Fan.h"
#include "Pinout.h"

Fan::Fan():myServo() {
}

void Fan::init() {
  myServo.attach(FAN_PIN,1000,2000);
  myServo.write(0);
  delay(100);
  myServo.write(90);
  delay(100);
  myServo.write(180);
  delay(100);
  myServo.write(0);
}

void Fan::setPwr(unsigned int newPwr) {
  myServo.write(map(newPwr,0,100,0,180));
}
