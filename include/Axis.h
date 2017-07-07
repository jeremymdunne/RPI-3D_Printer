#ifndef _AXIS_H
#define _AXIS_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
#include "A4988.h"
using namespace std;

class Axis{
public:
  Axis(int stepPin, int dirPin, float stepsPerRevolution, float stepsPerMM, bool isAxisInverted, int standardFeedrateMMperMin, int minSwitch, bool isSwitchInverted);
  void moveToPosition(float mm);
  void moveMM(float mm);
  float getPosition();
  int determineRPM(int feedrate);
  void setPosition(double mm); 
  void reset();
  void setSpeed(float feedrateMMperMin);
  void setPositionToZero();
private:
  A4988 *motor;
  float stepsPerMM;
  bool isAxisInverted;
  int feedrate; //MM per Min
  int minSwitch;
  bool isSwitchInverted;
  int stepsPerRevolution;
  float position = 0;

  int determineStepsToTravel(float endPosition);
  void moveSteps(int steps);
  bool isAtMin();
};


#endif
