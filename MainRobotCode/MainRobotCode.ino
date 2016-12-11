#define DEBUG_PRINT //used to enable debug print statements
#include <digitalIOPerformance.h>
#include <LiquidCrystal.h>
#include "DrivePWM.h"
#include "WallMath.h"
#include "PID.h"
#include "debug.h"
#include "Encoder.h"
#include "Rates.h"
#include "RobotOdometry.h"
#include "DriveController.h"
#include "GlobalInstances.h"

void setup() {
  DebugBegin();
  DebugPrintln("Serial started");
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
    //delay(1000);
    currentState = WALL_FOLLOW;
    break;
    case SPEC_CASE:
    mySpecCaseState->handle();
    break;
    case WALL_FOLLOW:
    //Serial.println("Test");
    myWallState -> handle();
    break;
    case TURN:
    myTurnState->handle();
    break;
    case FORWARD_DIST:
    myForwardState->handle();
    break;
    default:
    break;
  }
  myDriveControl->update();
  computeOdometry();
  manageLCD();
/*  DebugPrint(getXLoc()); //mm
  DebugPrint('\t');
  DebugPrint(getYLoc()); //mm
  DebugPrint('\t');
  DebugPrintln(getTheta()*(180/PI)); //deg*/
}

