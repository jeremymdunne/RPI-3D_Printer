#ifndef _A4988_CLASS
#define _A4988_CLASS
 
#include <iostream>
#include <fstream>
#include <wiringPi.h>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <iomanip>
#include <math.h>

class A4988 {
public:
	A4988(int stepsPerRev, int stpPin, int dirPin);
	void setStepperSpeed(int speed);
	void stepStepper(int steps);
private:
	int stpPin;
	int dirPin;
	int rpm;
	int delayTime;
	int stepsPerRev;
};


#endif
