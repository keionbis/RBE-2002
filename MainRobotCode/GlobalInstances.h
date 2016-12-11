#ifndef GLOBAL_INSTANCES_H
#define GLOBAL_INSTANCES_H

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
