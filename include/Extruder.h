#ifndef _Extruder_H
#define _Extruder_H

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
#include "Axis.h"
#include "Heater.h"
using namespace std;
class Extruder{
public:
  Extruder(int stepPin, int dirPin,int stepsPerRev,double stepsPerMM,int standardSpeed,bool isAxisInverted, int heaterPin, int thermisterPin);
  void extrudeMM(double amount);
  void extrudeToMM(double amountToGoTo);
  void setExtruderTemp(double temp);
  void getExtruderToTemp();
  void getExtruderToTemp(double temp);
  void setCoolDownTemp(double temp);
  void startCoolDown(double temp);
  void fakeExtrudeDist(double amount);
  void fakeExtrudeToMM(double amount); 
  bool hasCooledDown();
  void startCoolDown();
  void waitUntilCool();
  float getTemp();
  float getRunningExtrudedAmount();
  float getTotalExtrudedAmount();
  void resetExtrudedAmount();

private:
  float totalExtrudeAmount = 0;
  float runningExtrudedAmount = 0;
  Heater *heater;
  Axis *extruderMotor;
};


#endif
