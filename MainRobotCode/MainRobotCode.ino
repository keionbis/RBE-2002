#define DEBUG_PRINT //used to enable debug print statements
#include <digitalIOPerformance.h>
#include <LiquidCrystal.h>
#define SDA_PORT PORTD
#define SDA_PIN 3
#define SCL_PORT PORTD
#define SCL_PIN 5
#define I2C_NOINTERRUPT 1
#include <SoftWire.h>
#include "DrivePWM.h"
#include "WallMath.h"
#include "PID.h"
#include "debug.h"
#include "Encoder.h"
#include "Rates.h"
#include "RobotOdometry.h"
#include "DriveController.h"

#include "GlobalInstances.h"
#include "DFRobotIRPosition.h"
#include "IRCamera.h"


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
    delay(1000);
    currentState = WALL_FOLLOW;
    break;
    case SPEC_CASE:
    mySpecCaseState->handle();
    break;
    case WALL_FOLLOW:
    myWallState -> handle();
    sweepIRCamera();
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
  computeOdometry(myDriveControl);
  manageLCD();
}

