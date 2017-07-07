

#include "Axis.h"


Axis::Axis(int stepPin, int dirPin, float stepsPerRevolution, float stepsPerMM, bool isAxisInverted, int standardFeedrateMMperMin, int minSwitch, bool isSwitchInverted){
  cout << " Called "<< endl;
  this->motor = new A4988(stepsPerRevolution, stepPin, dirPin);
  cout << "motor made " << endl;
  this->stepsPerRevolution = stepsPerRevolution;
  this->stepsPerMM = stepsPerMM;
  this->isAxisInverted = isAxisInverted;
  this->feedrate = standardFeedrateMMperMin;
  cout << "Speed: " <<   this->feedrate << endl;
  this->setSpeed(this->feedrate);
  if(minSwitch != NULL)
  pinMode(minSwitch, INPUT);
  this->minSwitch = minSwitch;
  this->isSwitchInverted = isSwitchInverted;
}

void Axis::setPosition(double amount){
  this->position = amount;
}
void Axis::setPositionToZero(){
  this->position = 0;
}

void Axis::moveMM(float mm){
  this->moveSteps(mm*stepsPerMM);
}

void Axis::moveToPosition(float mm){
  this->moveSteps(this->determineStepsToTravel(mm));
}

void Axis::setSpeed(float feedrateMMperMin){
  //cout << "Setting speed: " << endl;
  feedrate = feedrateMMperMin;
  int speed = determineRPM(feedrate);

  motor->setStepperSpeed(speed);
  //cout << "Speed set" << endl;
}

float Axis::getPosition(){
  return this->position;
}

void Axis::reset(){
  //go to position 0
  //cout <<  "SwitchInversion: " << this->isSwitchInverted << endl;
  //cout << "Min Status: " << this->isAtMin() << endl;
  while(this->isAtMin() == 0){
    //cout << "Moving: " << endl;
    this->moveSteps(-100);
    //cout << " DONE " << endl;
  }
  //cout << " Done resetting " << endl;
  this->position = 0;
}

bool Axis::isAtMin(){
  if(this->isSwitchInverted){
    return !digitalRead(this->minSwitch);
  }
  else{
    return digitalRead(this->minSwitch);
  }
}

int Axis::determineRPM(int feedrate){
  //convert a MM per Min to RPM
  float stepsPerMin = (float)feedrate * this->stepsPerMM;
  int rpm = (int)(stepsPerMin/this->stepsPerRevolution + .5);
  return rpm;
}


int Axis::determineStepsToTravel(float endPosition){
  //cout << " EP: " << endPosition << endl;
  float deltaMM = endPosition - position;
  if(deltaMM == 0) return 0;
  float deltaSteps = deltaMM * stepsPerMM;
  //cout << "DPL " << deltaSteps << endl;
  if(deltaSteps < 0){
    return (int)(deltaSteps - .5);
  }
  else{
    return (int)(deltaSteps + .5);
  }
}

void Axis::moveSteps(int steps){
  if(steps == 0) return;

  //cout << "Called with: " << steps << endl;
  //int speed = this->determineRPM(this->feedrate);
  //cout << "RPM:  " << speed << endl;
  //cout << "MM Speed: " << feedrate << endl;
    //this->motor->setStepperSpeed(speed);
    //cout << "Speed set:" << endl;
    //cout << "S/M: " << stepsPerMM << endl;
  this->position += float(steps)/float(stepsPerMM);
  //cout << steps << endl;
  //cout <<  float(steps)/float(stepsPerMM) << endl;
  //cout << "Position: " << position << endl;
  if(this->isAxisInverted) steps = steps * -1;
  this->motor->stepStepper(steps);
  //cout << "Moved: " << endl;
}
