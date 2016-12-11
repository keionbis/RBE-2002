#include "IRCamera.h"
#include "GlobalInstances.h"
IRCamera::IRCamera() {
  
}
IRCamera::IRTarget IRCamera::getTarget() { //returns the flame target or -1,-1 if no valid targets
  IRTarget newTarget;
  newTarget.xPos = -1;
  newTarget.yPos = -1;
  newTarget.size = -1;
  readTargets(); //updates targets
  for(int i = 0;i<4;i++)
  {
    if((targets[i].xPos <1023)&&(targets[i].xPos >0)&&(targets[i].yPos <1023)&&(targets[i].yPos >0))
    {
      newTarget.xPos = targets[i].xPos;
      newTarget.yPos = targets[i].yPos;
      newTarget.size = targets[i].size;
      return newTarget;
    }
    /*Serial.print(targets[i].xPos);
    Serial.print(',');
    Serial.print(targets[i].yPos);
    Serial.print('\t');*/
  }
  Serial.println(' ');
  return newTarget;
}
void IRCamera::init() { //sets up camera
  I2Cbegin();
  writeTwoIICByte(0x30,0x01);
  delay(10);
  writeTwoIICByte(0x30,0x08);
  delay(10);
  writeTwoIICByte(0x06,0x90);
  delay(10);
  writeTwoIICByte(0x08,0xC0);
  delay(10);
  writeTwoIICByte(0x1A,0x40);
  delay(10);
  writeTwoIICByte(0x33,0x33);
  delay(10);
  delay(100);
}
void IRCamera::readTargets() {
    //IR sensor read
    byte data_buf[16];
    I2CbeginTransmission(ADDRESS);
    I2Cwrite(0x36);
    I2CendTransmission();
    I2CrequestFrom(ADDRESS, 16);        // Request the 2 byte heading (MSB comes first)
    int i = 0;
    while(I2Cavailable() && i < 16) {
        data_buf[i] = I2Cread();
        /*char str[5];
        sprintf(str,"%02X",data_buf[i]);
        Serial.print(str);*/
        i++;
    }
    targets[0].size = data_buf[3];
    targets[0].xPos = ((data_buf[3] & 0x30) <<4)|data_buf[1];
    targets[0].yPos = ((data_buf[3] & 0xC0) <<2)|data_buf[2];
    
    targets[1].size = data_buf[6];
    targets[1].xPos = ((data_buf[6] & 0x30) <<4)|data_buf[4];
    targets[1].yPos = ((data_buf[6] & 0xC0) <<2)|data_buf[5];

    targets[2].size = data_buf[9];
    targets[2].xPos = ((data_buf[9] & 0x30) <<4)|data_buf[7];
    targets[2].yPos = ((data_buf[9] & 0xC0) <<2)|data_buf[8];
 
    targets[3].size = data_buf[12];
    targets[3].xPos = ((data_buf[12] & 0x30) <<4)|data_buf[10];
    targets[3].yPos = ((data_buf[12] & 0xC0) <<2)|data_buf[11];
}
void IRCamera::writeTwoIICByte(uint8_t first, uint8_t second)
{
  I2CbeginTransmission(ADDRESS);
  I2Cwrite(first);
  I2Cwrite(second);
  I2CendTransmission();
}
