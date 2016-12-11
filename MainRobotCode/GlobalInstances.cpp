#include "GlobalInstances.h"
extern RobotState currentState = STANDBY;
extern RobotState previousState = STANDBY;

extern DriveController* myDriveControl=NULL;
extern WallFollowState* myWallState=NULL;
extern ForwardState* myForwardState=NULL;
extern TurnState* myTurnState=NULL;
extern SpecCaseState* mySpecCaseState=NULL;

extern void initalizeInstances() {
  myDriveControl = DriveController::getInstance();
  myDriveControl->setup();
  myWallState = WallFollowState::getInstance();
  myForwardState = ForwardState::getInstance();
  myTurnState = TurnState::getInstance();
  mySpecCaseState = SpecCaseState::getInstance();
}
extern SoftWire mySoftwire = SoftWire();
