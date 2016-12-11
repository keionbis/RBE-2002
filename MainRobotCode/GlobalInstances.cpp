#include "GlobalInstances.h"
extern RobotState currentState = STANDBY;
extern RobotState previousState = STANDBY;

extern DriveController* myDriveControl=NULL;
extern WallFollowState* myWallState=NULL;
extern ForwardState* myForwardState=NULL;
extern TurnState* myTurnState=NULL;
extern SpecCaseState* mySpecCaseState=NULL;
