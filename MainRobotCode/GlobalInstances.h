#if ~defined(GLOBAL_INSTANCES_H)
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

static void initalizeInstances() {
  myDriveControl = DriveController::getInstance();
  myDriveControl->setup();
  myWallState = WallFollowState::getInstance();
  myForwardState = ForwardState::getInstance();
  myTurnState = TurnState::getInstance();
  mySpecCaseState = SpecCaseState::getInstance();
}

enum RobotState {
  WALL_FOLLOW=1,
  TURN=2,
  STANDBY=3,
  SPEC_CASE=4, //Detected that front wall sensor maxed and other isn't
  FORWARD_DIST=5
};

extern RobotState currentState;
extern RobotState previousState;

#endif
