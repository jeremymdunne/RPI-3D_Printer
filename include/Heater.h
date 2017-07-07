#ifndef _Heater_CLASS
#define _Heater_CLASS

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
#include <thread>
#include <math.h>
#include "timer.h"

using namespace std;
using namespace timer;

class Heater{

public:
  Heater(int heaterPin, int thermisterPin);

  void setCoolDownTemp(int temp);
  void startHeating();
  void setTargetTemp(int temp);
  void startCoolDown();
  float getTemp();
  bool hasCooledDown();
  bool hasReachedTemp();
  void runTime();
private:
  void loop();
  bool shouldHeat = false;
  bool isHeaterOn = false;
  float thermisterFudgeFactor = 1.94;
  int curTemp;
  int targetTemp;
  int tempToCooldownTo = 60;
  int thermisterPin;
  int heaterPin;
  float determineTemp();
  float resistanceToTempConvertion(int resistance);
  float getThermisterChargeTime();
  float calculateResistanceOfThermister(double chargeTime);
  void turnHeaterOn();
  void turnHeaterOff();
};

#endif
