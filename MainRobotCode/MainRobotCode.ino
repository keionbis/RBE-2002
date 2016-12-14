#define DEBUG_PRINT //used to enable debug print statements
#include <digitalIOPerformance.h>
#include <LiquidCrystal.h>
#include <SoftWire.h>
#include <SoftI2CMaster.h>
#include <Servo.h>
//#include "Fan.h"
#include "DrivePWM.h"
#include "WallMath.h"
#include "PID.h"
#include "debug.h"
#include "Encoder.h"
#include "Rates.h"
#include "RobotOdometry.h"
#include "DriveController.h"
#include "TurnState.h"
#include "Fan.h"
#include "Pinout.h"

#include "GlobalInstances.h"
#include "IRCamera.h"

SoftWire* Wire = SoftWire::getInstance();
void handleCandleSearch();

Servo pan;
Servo tilt;

Fan* fan;

void setup() {
  DebugBegin();
  DebugPrintln("Serial started");
  pinMode(START_BTN,INPUT_PULLUP);
  pinMode(BTN_LIGHT,OUTPUT);
  digitalWrite(BTN_LIGHT,LOW);
  delay(1000);
  while(digitalRead(START_BTN)) {delay(15);}
  digitalWrite(BTN_LIGHT,HIGH);
  Serial.println("Btn press");
  /*if (!gyro->init()) // gyro init
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1); 
  }
  gyro->enableDefault(); // gyro init. default 250/deg/s*/
  Serial.println("Gyro Done init");
// delay(1000);
  pan.attach(7);
  tilt.attach(11);
  pan.write(40);
  tilt.write(50);
  fan = Fan::getInstance();
  fan->init();
  Serial.println("Fan init");
  IRCamera::getInstance() -> init();
  lcd.begin(16,2);
  lcd.print("Standby");
  lcd.setCursor(0,1);
  lcd.print("Hello World!");
  wallInit();
  initalizeInstances();
  for(int i = 0;i<100;i+=5)
  {
    fan->setPwr(i);
    delay(15);
  }  
}
bool returningHome = false;
void loop() {
  if(currentState == STANDBY)
  { //add btn later
    delay(1000);
    currentState = WALL_FOLLOW;
  }
  else if(currentState == SPEC_CASE)
  {
    mySpecCaseState->handle();
  }
  else if(currentState == WALL_FOLLOW)
  {
    myWallState -> handle();
  }
  else if(currentState == TURN)
  {
    myTurnState->handle();
  }
  else if(currentState == FORWARD_DIST)
  {
    myForwardState->handle();
  }
  else if(currentState == CANDLE)
  {
    static long long cTime = 0;
    static int X_SET = 384; 
    static int Y_SET = 512;
    static int tiltOut  = 45;
    static int panOut  = 50;
    if(millis()-cTime>75)
    {
      cTime = millis();
      static IRCamera::IRTarget newTarget = IRCamera::getInstance()->getTarget();
      newTarget = IRCamera::getInstance()->getTarget();
      if(newTarget.xPos!=-1)
      {
        int xError = newTarget.xPos-X_SET;
        int yError = newTarget.yPos-Y_SET;
        int diffX = xError/25;
        int diffY = yError/25;
        tiltOut-=diffX;
        tiltOut = constrain(tiltOut,45,75);
        tilt.write(tiltOut);
        panOut-=diffY;
        panOut = constrain(panOut,50,170);
        pan.write(panOut);
      }
    }
    static long long mTime = 0;
    static bool doneCandle = false;
    if(millis()-mTime > 10 && !doneCandle)
    {
      mTime = millis();
      int error = 125-panOut; //pos-left of center
      float turnPID = error/125.0; 
      myDriveControl->setBothSetpoints(-1*DriveController::DEFAULT_SETPOINT*turnPID,DriveController::DEFAULT_SETPOINT*turnPID);
      //Serial.println(panOut);
      if(abs(panOut-125) < 5)
      {
        myDriveControl->setBothSetpoints(DriveController::DEFAULT_SETPOINT*0.5,DriveController::DEFAULT_SETPOINT*.5);
  /*      for(int i = 0;i<100;i++)
        {
          fan->setPwr(i);
          delay(10);
        }
        fan->setPwr(0);*/
        WallState newWallState = getWallState(RIGHT_WALL);
        if(newWallState.frontDist >0)
        {
          myDriveControl->setBothSetpoints(0,0);
          WallState newWallState = getWallState(RIGHT_WALL);
          float angle = (-0.022481503*panOut) + 2.898327002;
          float x_2 = 2.5+(-1*cos(PI-angle)*2.25);
          float y_2 = 5.375+(sin(PI-angle)*2.25);
          float x_3 = (0.0220155514*newWallState.frontDist)+9.661633531;
          candlez = y_2+ (tan((PI/2.0)-angle)*(x_3-x_2));
          //candle in range
          candlex = cos(getTheta())*(newWallState.frontDist+(4.5*25.4)+(1.5*25.4));
          candley = sin(getTheta())*(newWallState.frontDist+(4.5*25.4)+(1.5*25.4));
          candlex += getXLoc();
          candley += getYLoc();
          candlex = candlex/25.4;
          candley = candley/25.4;
          fan->setPwr(100);
          delay(1000);
          myDriveControl->setBothSetpoints(0,0);
          doneCandle = true;
          previousState = currentState;
          currentState = RWALL;
          fan->setPwr(0);
        }
        Serial.print(newWallState.frontDist);
        Serial.println("Centered at Camera");
      }
    }
  }
  else if(currentState == RWALL)
  {
    static bool finishedTurnAround = false;
    if(!finishedTurnAround)
    {
      finishedTurnAround = true;
      myTurnState->setTurnHeading(getTheta()+PI);
      previousState = currentState;
      currentState = TURN;
    }
    else
    {
      WallState newWallState = getWallState(RIGHT_WALL);
      myDriveControl->setBothSetpoints(DriveController::DEFAULT_SETPOINT,DriveController::DEFAULT_SETPOINT);
      if(newWallState.frontDist>0)
      {
        myDriveControl->setBothSetpoints(0,0);
        previousState = WALL_FOLLOW;
        currentState = TURN;
        returningHome = true;
        myTurnState -> setTurnHeading(leftWallTheta);
      }
    }
  }
  if(returningHome)
  {
    if(abs(getXLoc()) <(25.4*3) && abs(getYLoc()) <(25.4*3))
    {
      myDriveControl->setBothSetpoints(0,0);
      currentState = DONE;
    }
  }
  myDriveControl->update();
  computeOdometry();
  manageLCD();
  //Serial.println(getXLoc());
  handleCandleSearch();
}

void handleCandleSearch() {
  static bool setCandle = false;
  if(currentState != CANDLE && !setCandle)
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
    if(abs(newTarget.xPos-767.5)<10 || abs(newTarget.yPos-383.625)<10 )
    {
      myDriveControl->setBothSetpoints(0,0);
      previousState = currentState;
      currentState = CANDLE;
      setCandle = true;
      leftWallX = getXLoc();
      leftWally = getYLoc();
      leftWallTheta = getTheta();
      Serial.print(newTarget.xPos);
      Serial.print('\t');
      Serial.println(newTarget.yPos);
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
extern bool I2CendTransmission() {
  return Wire->endTransmission();
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



