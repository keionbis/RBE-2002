#define DEBUG_PRINT //used to enable debug print statements
#include <digitalIOPerformance.h>
#include <LiquidCrystal.h>
#include <SoftWire.h>
#include <SoftI2CMaster.h>
#include <Servo.h>
#include "DrivePWM.h"
#include "WallMath.h"
#include "PID.h"
#include "debug.h"
#include "Encoder.h"
#include "Rates.h"
#include "RobotOdometry.h"
#include "DriveController.h"

#include "GlobalInstances.h"
#include "IRCamera.h"

SoftWire* Wire = SoftWire::getInstance();
void handleCandleSearch();

Servo pan;
Servo tilt;
void setup() {
  pan.attach(7);
  tilt.attach(11);
  pan.write(40);
  tilt.write(50);
  DebugBegin();
  DebugPrintln("Serial started");
  IRCamera::getInstance() -> init();
  lcd.begin(16,2);
  lcd.print("Standby");
  lcd.setCursor(0,1);
  lcd.print("Hello World!");
  wallInit();
  initalizeInstances();
}

void loop() {
  switch(currentState)
  {
    case STANDBY://add btn later
    delay(1000);
    currentState = WALL_FOLLOW;
    break;
    case SPEC_CASE:
    mySpecCaseState->handle();
    break;
    case WALL_FOLLOW:
    myWallState -> handle();
    break;
    case TURN:
    myTurnState->handle();
    break;
    case FORWARD_DIST:
    myForwardState->handle();
    break;
    case CANDLE:
    break;
    default:
    break;
  }
  myDriveControl->update();
  computeOdometry();
  manageLCD();
  Serial.println(getXLoc());
  //handleCandleSearch();
}

void handleCandleSearch() {
  if(currentState != CANDLE)
  {
    static int tPos = 45;
    pan.write(35);
    static long long lastChange = 0;
    static bool goingUp = true;
    if((millis()-lastChange) > 10)
    {
      lastChange = millis();
      if(goingUp)
        tPos++;
      else
        tPos--;
      
      if(tPos>74)
        goingUp = false;
      else if(tPos<44)
        goingUp = true;
      tilt.write(tPos);
    }
    IRCamera::IRTarget newTarget = IRCamera::getInstance()->getTarget();
    /*Serial.print(newTarget.xPos);
    Serial.print(',');
    Serial.println(newTarget.yPos);*/
    if(newTarget.xPos !=-1)
    {
      myDriveControl->setBothSetpoints(0,0);
      previousState = currentState;
      currentState = CANDLE;
    } 
  }
}


extern void I2Cbegin() {
  Wire->begin();
}

extern void I2CrequestFrom(uint8_t address, uint8_t quantity) {
  Wire->requestFrom(address, quantity);
}
extern void I2CbeginTransmission(byte address) {
  Wire->beginTransmission(address);
}
extern void I2CendTransmission() {
  Wire->endTransmission();
}
extern void I2Cwrite(byte value) {
  Wire->write(value);
}
extern bool I2Cavailable() {
  return Wire->available();
}
extern byte I2Cread() {
  return Wire->read();
}



