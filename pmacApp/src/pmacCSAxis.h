/*
 * pmacCSAxis.h
 *
 *  Created on: 29 Feb 2016
 *      Author: gnx91527
 */

#ifndef PMACAPP_SRC_PMACCSAXIS_H_
#define PMACAPP_SRC_PMACCSAXIS_H_

#include "asynMotorController.h"
#include "asynMotorAxis.h"
#include "pmacCallbackInterface.h"
#include "pmacCommandStore.h"
#include "pmacDebugger.h"

class pmacCSController;

class pmacCSAxis : public asynMotorAxis, pmacCallbackInterface, public pmacDebugger
{
public:
  pmacCSAxis(pmacCSController *pController, int axisNo);
  virtual ~pmacCSAxis();
  asynStatus move(double position, int relative, double min_velocity, double max_velocity, double acceleration);
  asynStatus moveVelocity(double min_velocity, double max_velocity, double acceleration);
  asynStatus home(double minVelocity, double maxVelocity, double acceleration, int forwards);
  asynStatus stop(double acceleration);
  bool getMoving();
  double getCurrentPosition();

  virtual void callback(pmacCommandStore *sPtr, int type);

private:
  pmacCSController *pC_;

  asynStatus getAxisStatus(pmacCommandStore *sPtr);
  asynStatus getAxisInitialStatus(void);

  int deferredMove_;
  int scale_;
  double position_;
  double previous_position_;
  int previous_direction_;
  epicsTimeStamp nowTime_;
  epicsFloat64 nowTimeSecs_;
  epicsFloat64 lastTimeSecs_;
  bool printNextError_;
  bool moving_; // only valid within poll time - used as a hint for validating deferred coordinated moves

  friend class pmacCSController;
  friend class pmacCsGroups;

};

#endif /* PMACAPP_SRC_PMACCSAXIS_H_ */
