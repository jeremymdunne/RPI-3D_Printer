#include "Extruder.h"


Extruder::Extruder(int stepPin, int dirPin,int stepsPerRev,double stepsPerMM,int standardSpeed, bool isAxisInverted, int heaterPin, int thermisterPin){
  heater = new Heater(heaterPin, thermisterPin);
  extruderMotor = new Axis(stepPin, dirPin, stepsPerRev, stepsPerMM, isAxisInverted, standardSpeed, NULL, NULL);
}

float Extruder::getTotalExtrudedAmount(){
  return totalExtrudeAmount;
}

void Extruder::extrudeMM(double amount){
  //heater->runTime();
  extruderMotor->moveMM(amount);
  totalExtrudeAmount += amount;
  runningExtrudedAmount += amount;
}

void Extruder::fakeExtrudeToMM(double amount){
  totalExtrudeAmount += amount- extruderMotor->getPosition();
  runningExtrudedAmount = amount;
  extruderMotor->setPosition(amount);
}
void Extruder::fakeExtrudeDist(double amount){
  totalExtrudeAmount += amount;
  runningExtrudedAmount += amount;
  extruderMotor->setPosition(amount);
}
void Extruder::extrudeToMM(double amountToGoTo){
  totalExtrudeAmount += amountToGoTo - extruderMotor->getPosition();
  float amount = amountToGoTo - extruderMotor->getPosition();
  //cout <<"Extruder Position Pre Move: " << extruderMotor->getPosition() << endl;
  runningExtrudedAmount = amountToGoTo;
  this->extruderMotor->moveToPosition(amountToGoTo);
  //cout << "Extruding : " << amount << endl;
  //heater->runTime();
}
float Extruder::getRunningExtrudedAmount(){
  return runningExtrudedAmount;
}

void Extruder::resetExtrudedAmount(){
  this->runningExtrudedAmount = 0;
  extruderMotor->setPositionToZero();
}

void Extruder::setExtruderTemp(double temp){
  this->heater->setTargetTemp(temp);
}
float Extruder::getTemp(){
	//cout << "Getting temp of Extruder!" << endl;
  return this->heater->getTemp();
}
void Extruder::getExtruderToTemp(double temp){
  heater->setTargetTemp(temp);
  heater->startHeating();
  while(!this->heater->hasReachedTemp()){
    heater->runTime();
    cout << "Temp: " << heater->getTemp() << endl;
  }
}

bool Extruder::hasCooledDown(){
  return heater->hasCooledDown();
}

void Extruder::getExtruderToTemp(){
  this->heater->startHeating();
  while(!this->heater->hasReachedTemp());
}

void Extruder::setCoolDownTemp(double temp){
  this->heater->setCoolDownTemp(temp);
}

void Extruder::startCoolDown(double temp){
  this->heater->setCoolDownTemp(temp);
  this->heater->startCoolDown();
}

void Extruder::startCoolDown(){
  this->heater->startCoolDown();
}

void Extruder::waitUntilCool(){
  while(!this->heater->hasCooledDown());
}
