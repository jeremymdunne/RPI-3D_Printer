#include "Heater.h"


Heater::Heater(int heaterPin, int thermisterPin){
  this->thermisterPin = thermisterPin;
  this->heaterPin = heaterPin;
  pinMode(thermisterPin, INPUT);
  pinMode(heaterPin, OUTPUT);
}
bool Heater::hasCooledDown(){
  this->curTemp = this->determineTemp();
  if(this->curTemp <this->tempToCooldownTo) return true;
  return false;
}

void Heater::startCoolDown(){
  this->shouldHeat = false;
}

float Heater::getTemp(){
  return this->determineTemp();
}

bool Heater::hasReachedTemp(){
  if(this->curTemp >= this->targetTemp) return true;
  return false;
}
void Heater::turnHeaterOn(){
  digitalWrite(heaterPin, HIGH);
  this->isHeaterOn = true;
}
void Heater::turnHeaterOff(){
  digitalWrite(this->heaterPin, LOW);
  this->isHeaterOn = false;
}
void Heater::loop(){
  while(shouldHeat){
    //get temp
    curTemp = determineTemp();
    if(curTemp < targetTemp)
      if(!this->isHeaterOn) this->turnHeaterOn();
    if(curTemp > targetTemp)
      if(this->isHeaterOn) this->turnHeaterOff();
      delay(500);
  }
  while(this->curTemp > this->tempToCooldownTo){
      this->curTemp = this->determineTemp();
      if(this->isHeaterOn) this->turnHeaterOff();
      delay(500);
  }
}

void Heater::runTime(){
  if(shouldHeat){
    curTemp = determineTemp();
    if(curTemp < targetTemp)
      if(!this->isHeaterOn) this->turnHeaterOn();
    if(curTemp > targetTemp)
      if(this->isHeaterOn) this->turnHeaterOff();
  }
  else{
    curTemp = determineTemp();
    if(isHeaterOn) turnHeaterOff();
  }
}
void Heater::startHeating(){
  this->shouldHeat = true;
  runTime();
  thread heatThread(&Heater::loop, this);
  //thread heatThread = new thread([this]{loop();});
  heatThread.detach();
}
void Heater::setCoolDownTemp(int temp){
  this->tempToCooldownTo = temp;
  turnHeaterOff();
}
void Heater::setTargetTemp(int temp){
  this->targetTemp = temp;
}
float Heater::determineTemp(){
  double chargeTime = this->getThermisterChargeTime();
  double resistance = this->calculateResistanceOfThermister(chargeTime);
  double temp = this->resistanceToTempConvertion(resistance);
  return temp;
}
float Heater::getThermisterChargeTime(){
  //record how long it took the capaciter to charge and fire for a simple rc=t circuit
  pinMode(this->thermisterPin, OUTPUT);
  digitalWrite(this->thermisterPin, LOW);
  delay(1);
  pinMode(this->thermisterPin, INPUT);
  struct timespec start;
  clock_gettime(CLOCK_REALTIME, &start);
  while(!digitalRead(this->thermisterPin)) delayMicroseconds(10);
  return(getElapsedSeconds(&start));
}
float Heater::calculateResistanceOfThermister(double chargeTime){
  //t = rc, r = t/c
  double resistance = chargeTime / .00001;
  //cout << "Resistcane: " << resistance << endl;
  return resistance;
}
float Heater::resistanceToTempConvertion(int resistance){
    double temp = (double)((1.0/(.001125308852122L + (.000234711863267L* log(resistance))+(.000000085663516L * log(resistance))))-273.15)*this->thermisterFudgeFactor;
    return temp;
  }
