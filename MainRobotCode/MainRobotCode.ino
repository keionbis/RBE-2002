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
Servo pan;
Servo tilt;
void setup() {
  pan.attach(7);
  tilt.attach(4);
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
  
  for(int p = 70;p<=170;p+= 10)//(int p = 30;p<=170;p+= 10)
  {
    for(int t = 40;t<=75; t+=1)
    {
      pan.write(p);
      tilt.write(t);
      delay(75);
      float xAngleSamples[10];
      float yAngleSamples[10];
      float xSum = 0;
      float ySum = 0;
      int total = 0;
      for(int sample = 0;sample<10;sample++)
      {
        IRCamera::IRTarget newTarget = IRCamera::getInstance()->getTarget();
        xAngleSamples[sample] = newTarget.xAngle;
        yAngleSamples[sample] = newTarget.yAngle;
        xSum += newTarget.xAngle;
        ySum += newTarget.yAngle;
        total++;  
      }
      float xMean = xSum/(float)total;
      float yMean = ySum/(float)total;
      float xSumFiltered = 0;
      float ySumFiltered = 0;
      int totalFiltered = 0;
      for(int sample =0;sample<10;sample++)
      {
        if(xAngleSamples[sample] != -1)
        {
          //Good sample
          xSumFiltered  += xAngleSamples[sample];
          ySumFiltered  += yAngleSamples[sample];
          totalFiltered++;
        }
      } 
        float xMeanFiltered = xSumFiltered/(float)totalFiltered;
        float yMeanFiltered = ySumFiltered/(float)totalFiltered;
        Serial.print(p);
        Serial.print(',');
        Serial.print(t);
        Serial.print(',');
        Serial.print(xMeanFiltered);
        Serial.print(',');
        Serial.println(yMeanFiltered);
      
    }
    for(int t = 75;t>40; t-=1)
    {
            tilt.write(t);
            delay(10);
    }
  }
  //IRCamera::IRTarget newTarget = IRCamera::getInstance()->getTarget();
          
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



