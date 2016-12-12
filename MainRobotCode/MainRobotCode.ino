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
  pan.write(50);
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
  
  /*for(int p = 0;p<=110;p+= 10)
  {
    for(int t = 30;t<=90; t+=1)
    {
      pan.write(p);
      tilt.write(t);
      delay(15);
      int xPosSamples[10];
      int yPosSamples[10];
      long xSum = 0;
      long ySum = 0;
      int total = 0;
      for(int sample = 0;sample<10;sample++)
      {
        IRCamera::IRTarget newTarget = IRCamera::getInstance()->getTarget();
        xPosSamples[sample] = newTarget.xPos;
        yPosSamples[sample] = newTarget.yPos;
        xSum += newTarget.xPos;
        ySum += newTarget.yPos;
        total++;  
      }
      float xMean = xSum/(float)total;
      float yMean = ySum/(float)total;
      long xSumFiltered = 0;
      long ySumFiltered = 0;
      int totalFiltered = 0;
      for(int sample =0;sample<10;sample++)
      {
        if(!((abs(xMean-xPosSamples[sample]) > 100)||(abs(yMean-yPosSamples[sample]) > 100)||(xPosSamples[sample]<0)||(yPosSamples[sample]<0)))
        {
          //Good sample
          xSumFiltered  += xPosSamples[sample];
          ySumFiltered  += yPosSamples[sample];
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
  }*/
  IRCamera::IRTarget newTarget = IRCamera::getInstance()->getTarget();
          
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



