#ifndef DEBUG_H
#define DEBUG_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "RobotOdometry.h"
#include "GlobalInstances.h"

#if defined(DEBUG_PRINT)
  #define DebugBegin() Serial.begin(115200)
  #define DebugPrintln(a) Serial.println(a)
  #define DebugPrint(a) Serial.print(a)
#else
  #define DebugBegin() 
  #define DebugPrintln(a) 
  #define DebugPrint(a)
#endif  

static LiquidCrystal lcd(40,41,42,43,44,45); 

static void manageLCD() {
  static long long lastTime = 0;
  if(millis()-lastTime > 100)
  {
    lastTime = millis();
    lcd.clear();
    lcd.setCursor(0,0);
    char buf[25];
    long x = getXLoc()/25.4;
    long theta = (getTheta()*180/PI);
    //Serial.println(theta);    
    char bufTwo[25];
    long y = (getYLoc()/25.4);
    long cx = candlex;
    long cy = candley;
    switch(currentState)
    {
      case STANDBY:
      sprintf(buf,"x: %ld h: %ld",x,theta);
      sprintf(bufTwo,"y:%d stan",y);
      break;
      case WALL_FOLLOW:
      sprintf(buf,"x: %ld h: %ld",x,theta);
      sprintf(bufTwo,"y:%d wall",y);
      break;
      case FORWARD_DIST:
      sprintf(buf,"x: %ld h: %ld",x,theta);
      sprintf(bufTwo,"y:%d forw",y);
      break;
      case TURN:
      sprintf(buf,"x: %ld h: %ld",x,theta);
      sprintf(bufTwo,"y:%d turn",y);
      break;
      case SPEC_CASE:
      sprintf(buf,"x: %ld h: %ld",x,theta);
      sprintf(bufTwo,"y:%d spec",y);
      case CANDLE:
      sprintf(buf,"x: %ld h: %ld",cx,theta);
      sprintf(bufTwo,"y:%d candle",cy);
      default:
      break;
      
    }
    lcd.print(buf);
    lcd.setCursor(0,1);
    lcd.print(bufTwo);
  }
}

#endif
