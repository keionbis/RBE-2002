#ifndef GLOBAL_INSTANCES_H
#define GLOBAL_INSTANCES_H

#include "DriveController.h"
#include "WallFollowState.h"
#include "ForwardState.h"
#include "TurnState.h"
#include "SpecCaseState.h"
#define SDA_PORT PORTD
#define SDA_PIN 3
#define SCL_PORT PORTD
#define SCL_PIN 5
#define I2C_NOINTERRUPT 1
#include <SoftWire.h>

extern DriveController* myDriveControl;
extern WallFollowState* myWallState;
extern ForwardState* myForwardState;
extern TurnState* myTurnState;
extern SpecCaseState* mySpecCaseState;
extern SoftWire mySoftwire;

extern void initalizeInstances();

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
