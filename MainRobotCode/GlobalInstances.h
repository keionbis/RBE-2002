#ifndef GLOBAL_INSTANCES_H
#define GLOBAL_INSTANCES_H

#include "Arduino.h"
#include "DriveController.h"
#include "WallFollowState.h"
#include "ForwardState.h"
#include "TurnState.h"
#include "SpecCaseState.h"

extern DriveController* myDriveControl;
extern WallFollowState* myWallState;
extern ForwardState* myForwardState;
extern TurnState* myTurnState;
extern SpecCaseState* mySpecCaseState;
extern void initalizeInstances();

extern void I2Cbegin();
extern void I2CrequestFrom(uint8_t address, uint8_t quantity);
extern void I2CbeginTransmission(byte address);
extern void I2CendTransmission();
extern void I2Cwrite(byte value);
extern bool I2Cavailable();
extern byte I2Cread();

typedef enum RobotState {
  WALL_FOLLOW,
  TURN,
  STANDBY,
  SPEC_CASE, //Detected that front wall sensor maxed and other isn't
  FORWARD_DIST
};

extern RobotState currentState;
extern RobotState previousState;

#endif
