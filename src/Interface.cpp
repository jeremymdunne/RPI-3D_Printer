
#include "Interface.h"
#define PRINTER_3D "3DPRINTER"
float Interface::getRunningExtrudedAmount(){

  return extruder1->getRunningExtrudedAmount();

}
float Interface::getTotalExtrudedAmount(){

  return extruder1->getTotalExtrudedAmount();

}
void Interface::turnOn(){
  if(shouldHeat || shouldMove){
    pinMode(psuPin, OUTPUT);
    digitalWrite(psuPin, HIGH);
    delay(1000);
  }
}
void Interface::setExtruderTemp(int temp){
  if(shouldHeat)
  extruder1->setExtruderTemp(temp);
cout << "Temp set!" << endl;
}

InterfaceInformation Interface::getInformation(){
  InterfaceInformation info;
  info.position = getPosition();
  //cout << "Getting temp...;" << endl;
  if(shouldHeat) info.temperature = (float)extruder1->getTemp();

  else info.temperature = 0;
   //cout << "Got temp..." << endl;
  info.extrudedAmount = extruder1->getTotalExtrudedAmount();
  return info;
}

void Interface::getExtruderToTemp(){
	cout << "Getting to temp! " << endl;
  if(shouldHeat){
  extruder1->getExtruderToTemp();
  //cout << "Getting to temp!" << endl;
  }
}

void Interface::getExtruderToTemp(int temp){
	cout << "Getting to temp! " << endl;
  if(shouldHeat){
  extruder1->getExtruderToTemp(temp);
  cout << "At Temp!" << endl;
}
}

void Interface::safe(){
  resetAllAxis();

	  coolDownExtruder();
		while(!extruder1->hasCooledDown() && sysType == PRINTER_3D) cout << "Cool Down Temp: " << extruder1->getTemp() << endl;

  turnOff();
}

void Interface::coolDownExtruder(){

	extruder1->setCoolDownTemp(60);//
	extruder1->startCoolDown();

}


void Interface::emergencyStop(){
  turnOffPower();
}

void Interface::turnOff(){
  //TODO
      extruder1->startCoolDown(60);
    while(extruder1->getTemp() > 60);

  turnOffPower();
}

void Interface::turnOffPower(){
//  if(shouldMove || shouldHeat)
  digitalWrite(psuPin, LOW);
}

void Interface::resetAllAxis(){
  if(shouldMove){
  xMotor->reset();
  yMotor->reset();
  //z is special as it has an offset and is hall effect controlled
  //raise
  zMotor->moveToPosition(zRaiseBeforeReset);
  //call zero
 zMotor->reset();
 //move to offset position
 zMotor->moveToPosition(zAxisOffset);
 //reset to call this zero
 zMotor->setPositionToZero();
}
}
Point3d Interface::getPosition(){
  Point3d position;
  position.x = xMotor->getPosition();
  position.y = yMotor->getPosition();
  position.z = zMotor->getPosition();
  return position;
}
void Interface::resetAxis(int axis){
  if(shouldMove){
  if(axis == X_AXIS) xMotor->reset();
  else if(axis == Y_AXIS) yMotor->reset();
  else if(axis == Z_AXIS){
    //z is special as it has an offset and is hall effect controlled
    //raise
    zMotor->moveToPosition(zRaiseBeforeReset);
    //call zero
   zMotor->reset();
   //move to offset position
   zMotor->moveToPosition(zAxisOffset);
   //reset to call this zero
   zMotor->setPositionToZero();
 }
}
}

void Interface::zeroExtruder(){
  extruder1->resetExtrudedAmount();
}

/*
void Interface::moveToPosition(MovementCommand command){

  cout << "Position: ";
  Point3d curPos = getPosition();
  cout << "X: " << curPos.x << " Y: " << curPos.y << " Z: " << curPos.z << endl;
  cout << "To: ";
  float curExtrudedAmount = getRunningExtrudedAmount();
  command.toString();
  //delay(500);
  thread *xMovement;
  thread *yMovement;
  thread *zMovement;
  thread *eMovement;
  bool xUsed = false, yUsed = false, zUsed = false, eUsed = false;
  if(shouldMove){
    if(command.coordinateType == ABSOLUTE_COORDINATE){
      if(command.targetPosition.x != DNE && command.targetPosition.x != curPos.x){
        cout << "X" << endl;
        xUsed = true;
        //cout << "X: " << endl;
        if(command.targetSpeed.x < maxSpeed.x) xMotor->setSpeed(command.targetSpeed.x);
        else xMotor->setSpeed(maxSpeed.x);
        cout << "X speed: " << command.targetSpeed.x << endl;
        //xMovement = new thread(&Axis::moveToPosition,xMotor,command.targetPosition.x);
        xMotor->moveToPosition(command.targetPosition.x);
      }
      if(command.targetPosition.y != DNE && command.targetPosition.y != curPos.y){
        cout << "Y" << endl;
        cout << "Y speed: " << command.targetSpeed.y << endl;
      }
      if(command.targetPosition.z != DNE && command.targetPosition.z != curPos.z){
        cout << "Z" << endl;
        //zMotor->setSpeed(Z_SPEED);
        //cout << "Z: " << endl;
        //if(command.targetSpeed.z < maxSpeed.z) zMotor->setSpeed(command.targetSpeed.z);
        //else zMotor->setSpeed(maxSpeed.z);
        cout << "Z Speed: " << command.targetSpeed.z << endl;
        zMotor->moveToPosition(command.targetPosition.z);
      }
      if(command.extrudePosition != DNE && command.extrudePosition != curExtrudedAmount){
        cout << "E" << endl;
      }
    }





  }


}





void Interface::moveToPosition(MovementCommand command){
  //TODO test if DNE is a possibility to check if step is needed
  thread *xMovement;
  thread *yMovement;
  thread *zMovement;
  thread *eMovement;
  bool xUsed = false, yUsed = false, zUsed = false, eUsed = false;
  //cout <<"Threading!0" << endl;
  if(shouldMove){
  if(command.coordinateType == ABSOLUTE_COORDINATE){
    if(command.targetPosition.x != DNE){
      xUsed = true;
      //cout << "X: " << endl;
      if(command.targetSpeed.x < maxSpeed.x) xMotor->setSpeed(command.targetSpeed.x);
      else xMotor->setSpeed(maxSpeed.x);
      //xMovement = new thread(&Axis::moveToPosition,xMotor,command.targetPosition.x);
      xMotor->moveToPosition(command.targetPosition.x);
    }
    if(command.targetPosition.y != DNE){
      yUsed = true;
      //cout << "Y: " << endl;
      if(command.targetSpeed.y < maxSpeed.y) yMotor->setSpeed(command.targetSpeed.y);
      else yMotor->setSpeed(maxSpeed.y);
      //yMovement = new thread(&Axis::moveToPosition,yMotor,command.targetPosition.y);
      yMotor->moveToPosition(command.targetPosition.y);
    }
    if(command.targetPosition.z != DNE){
      //zMotor->setSpeed(Z_SPEED);
      //cout << "Z: " << endl;
      if(command.targetSpeed.z < maxSpeed.z) zMotor->setSpeed(command.targetSpeed.z);
      else zMotor->setSpeed(maxSpeed.z);
      zMotor->moveToPosition(command.targetPosition.z);
    }
    if(command.extrudePosition != DNE){
      eUsed = true;
      //cout << "E: " << endl;
      if(shouldHeat){
        //eMovement = new thread(&Extruder::extrudeToMM,extruder1,command.extrudePosition);
        extruder1->extrudeToMM(command.extrudePosition);
      }
      else{
        extruder1->fakeExtrudeToMM(command.extrudePosition);
        //eMovement = new thread(&Extruder::fakeExtrudeToMM,extruder1,command.extrudePosition);

      }
      //cout << "Extruding to: " << command.extrudePosition << endl;
    }

  }
  else if(command.coordinateType == RELATIVE_COORDINATE){
    //cout << "Relative MOVE! " << endl;
    if(command.targetPosition.x != DNE ){
      xMotor->setSpeed(command.targetSpeed.x);
      xMotor->moveMM(command.targetPosition.x);
    }
    if(command.targetPosition.y != DNE){
      yMotor->setSpeed(command.targetSpeed.y);
      yMotor->moveMM(command.targetPosition.y);
    }
    if(command.targetPosition.z != DNE){
      //zMotor->setSpeed(command.targetSpeed.z);
      zMotor->moveMM(command.targetPosition.z);
    }
    if(command.extrudePosition != DNE){
      if(shouldHeat)extruder1->extrudeMM(command.extrudePosition);
      else(extruder1->fakeExtrudeDist(command.extrudePosition));
    }

  }
  //if(xUsed) xMovement->join();
  //if(yUsed) yMovement->join();
  //if(zUsed) zMovement->join();
  //if(eUsed) eMovement->join();
}
else{
  if(command.coordinateType == ABSOLUTE_COORDINATE){
    if(command.targetPosition.x != DNE ){

      xMotor->setPosition(command.targetPosition.x);
    }
    if(command.targetPosition.y != DNE){
      yMotor->setPosition(command.targetPosition.y);
    }
    if(command.targetPosition.z != DNE){
      //zMotor->setSpeed(command.targetSpeed.z);
      zMotor->setPosition(command.targetPosition.z);
    }
    if(command.extrudePosition != DNE){
      if(shouldHeat)extruder1->extrudeMM(command.extrudePosition);
      else(extruder1->fakeExtrudeToMM(command.extrudePosition));
    }

  }

}
//cout << "Catching up!" << endl;
//cout << "X: " << xUsed << " Y: " << yUsed << " Z: " << zUsed << " E: " << eUsed << endl;

//cout << "Threads done! " << endl;
}
*/

void Interface::moveToPosition(MovementCommand command){
  //TODO test if DNE is a possibility to check if step is needed
  thread *xMovement;
  thread *yMovement;
  thread *zMovement;
  thread *eMovement;
  Point3d curPos = getPosition();
  bool xUsed = false, yUsed = false, zUsed = false, eUsed = false;
  //cout <<"Threading!0" << endl;
  //cout << "Current Pos: " << curPos.x << " Y: " << curPos.y << " Z: " << curPos.z << endl;
  if(shouldMove){
  if(command.coordinateType == ABSOLUTE_COORDINATE){
    if(command.targetPosition.x != DNE){
      xUsed = true;
      //cout << "X: " << command.targetPosition.x << endl;
      if(command.targetSpeed.x < maxSpeed.x) xMotor->setSpeed(command.targetSpeed.x);
      else xMotor->setSpeed(maxSpeed.x);
      xMovement = new thread(&Axis::moveToPosition,xMotor,command.targetPosition.x);
      //xMotor->moveToPosition(command.targetPosition.x);
    }
    if(command.targetPosition.y != DNE){
      yUsed = true;
      //cout << "Y: " << command.targetPosition.y << endl;
      if(command.targetSpeed.y < maxSpeed.y) yMotor->setSpeed(command.targetSpeed.y);
      else yMotor->setSpeed(maxSpeed.y);
      yMovement = new thread(&Axis::moveToPosition,yMotor,command.targetPosition.y);
      //yMotor->moveToPosition(command.targetPosition.y);
    }
    if(command.targetPosition.z != DNE && curPos.z != command.targetPosition.z){
      //zMotor->setSpeed(Z_SPEED);
      //cout << "Z: " << command.targetPosition.z<<endl;
      zUsed = true;
      if(command.targetSpeed.z < maxSpeed.z) zMotor->setSpeed(command.targetSpeed.z);
      else zMotor->setSpeed(maxSpeed.z);
      //cout << "Speed set" << endl;
      zMovement = new thread(&Axis::moveToPosition,zMotor,command.targetPosition.z);

    //    cout << "Cur Z: " << curPos.z << endl;
      //zMotor->moveToPosition(command.targetPosition.z);
    }

    if(command.extrudePosition != DNE){
      eUsed = true;
      //cout << "E: " << command.extrudePosition <<endl;
      if(shouldHeat){
        eMovement = new thread(&Extruder::extrudeToMM,extruder1,command.extrudePosition);
        //extruder1->extrudeToMM(command.extrudePosition);
      }
      else{
        //extruder1->fakeExtrudeToMM(command.extrudePosition);
        eMovement = new thread(&Extruder::fakeExtrudeToMM,extruder1,command.extrudePosition);
        //extruder1->extrudeToMM(command.extrudePosition);
      }
      //cout << "Extruding to: " << command.extrudePosition << endl;
    }
//cout << "Done here!"<< endl;
  }
  else if(command.coordinateType == RELATIVE_COORDINATE){
    //cout << "Relative MOVE! " << endl;
    if(command.targetPosition.x != DNE ){
      xMotor->setSpeed(command.targetSpeed.x);
      xMotor->moveMM(command.targetPosition.x);
    }
    if(command.targetPosition.y != DNE){
      yMotor->setSpeed(command.targetSpeed.y);
      yMotor->moveMM(command.targetPosition.y);
    }
    if(command.targetPosition.z != DNE){
      //zMotor->setSpeed(command.targetSpeed.z);
      zMotor->moveMM(command.targetPosition.z);
    }
    if(command.extrudePosition != DNE && sysType == PRINTER_3D){
		//cout << "Extruding" << endl;
      if(shouldHeat)extruder1->extrudeMM(command.extrudePosition);
      else(extruder1->fakeExtrudeDist(command.extrudePosition));
    }

  }
  //cout << "Joining threads" << endl;
  if(xUsed){
     xMovement->join();
     delete xMovement;
   }
   //cout << "X " << endl;
  if(yUsed){
     yMovement->join();
     delete yMovement;
   }
   //cout << "y" << endl;
  if(zUsed){
   zMovement->join();
   delete zMovement;

 }
 //cout << "Success to Z" << endl;
  if(eUsed){
     eMovement->join();
     delete eMovement;
   }

}
else{
  if(command.coordinateType == ABSOLUTE_COORDINATE){
    if(command.targetPosition.x != DNE ){

      xMotor->setPosition(command.targetPosition.x);
    }
    if(command.targetPosition.y != DNE){
      yMotor->setPosition(command.targetPosition.y);
    }
    if(command.targetPosition.z != DNE){
      //zMotor->setSpeed(command.targetSpeed.z);
      zMotor->setPosition(command.targetPosition.z);
    }
    if(command.extrudePosition != DNE && sysType == PRINTER_3D){
      if(shouldHeat)extruder1->extrudeMM(command.extrudePosition);
      else(extruder1->fakeExtrudeToMM(command.extrudePosition));
    }

  }

}
//cout << "Catching up!" << endl;
//cout << "X: " << xUsed << " Y: " << yUsed << " Z: " << zUsed << " E: " << eUsed << endl;

//cout << "Threads done! " << endl;
}

Interface::Interface(string hardwareSettingsLocation, vector <PrinterFlags> printFlags){

  //wiringPiSetup();
  for(int i = 0; i < printFlags.size(); i++){
  switch(printFlags[i]){
    case MOVEMENT_OFF:
      shouldMove = false;
      cout << "Not Moving: " <<endl;
      break;
    case HEATER_OFF:
      shouldHeat = false;
      cout << " not Heating " << endl;
      break;
    default:
     break;
  }
}
  //TODO PSU pin
  ConfigParser hardwareSettings(hardwareSettingsLocation);
  //set up three axis
  //temp variablessudo ./
  int stepPin,dirPin,limitSwitchPin,standardSpeed;
  float stepsPerMM,stepsPerRevolution;
  bool axisInversion, switchInversion;
  hardwareSettings.getValue("PSUPin", &psuPin);
  hardwareSettings.getValue("XMotorStepPin", &stepPin);
  hardwareSettings.getValue("XMotorDirPin", &dirPin);
  hardwareSettings.getValue("XStepsPerRevolution", &stepsPerRevolution);
  hardwareSettings.getValue("XStepsPerMM", &stepsPerMM);
  hardwareSettings.getValue("IsXAxisInverted", &axisInversion);
  hardwareSettings.getValue("IsXLimitInverted", &switchInversion);
  hardwareSettings.getValue("XStandardSpeed", &standardSpeed);
  hardwareSettings.getValue("XLimitPin", &limitSwitchPin);
  //create x axis
  xMotor = new Axis(stepPin, dirPin, stepsPerRevolution, stepsPerMM, axisInversion, standardSpeed, limitSwitchPin, switchInversion);

  hardwareSettings.getValue("YMotorStepPin", &stepPin);
  hardwareSettings.getValue("YMotorDirPin", &dirPin);
  hardwareSettings.getValue("YStepsPerRevolution", &stepsPerRevolution);
  hardwareSettings.getValue("YStepsPerMM", &stepsPerMM);
  hardwareSettings.getValue("IsYAxisInverted", &axisInversion);
  hardwareSettings.getValue("IsYLimitInverted", &switchInversion);
  hardwareSettings.getValue("YLimitPin", &limitSwitchPin);
  hardwareSettings.getValue("YStandardSpeed", &standardSpeed);
  //create x axis
  yMotor = new Axis(stepPin, dirPin, stepsPerRevolution, stepsPerMM, axisInversion, standardSpeed, limitSwitchPin, switchInversion);

  hardwareSettings.getValue("ZMotorStepPin", &stepPin);
  hardwareSettings.getValue("ZMotorDirPin", &dirPin);
  hardwareSettings.getValue("ZStepsPerRevolution", &stepsPerRevolution);
  hardwareSettings.getValue("ZStepsPerMM", &stepsPerMM);
  hardwareSettings.getValue("IsZAxisInverted", &axisInversion);
  hardwareSettings.getValue("IsZLimitInverted", &switchInversion);
  hardwareSettings.getValue("ZLimitPin", &limitSwitchPin);
  hardwareSettings.getValue("ZStandardSpeed", &standardSpeed);
  //create x axis
  zMotor = new Axis(stepPin, dirPin, stepsPerRevolution, stepsPerMM, axisInversion, standardSpeed, limitSwitchPin, switchInversion);
  //check system type
  hardwareSettings.getValue("SystemType", &sysType);
  cout << "System Type: " << sysType << endl;

    cout << "Creating Extruder 1 " << endl;
  //create extruder
  hardwareSettings.getValue("Extruder1StepPin", &stepPin);
  hardwareSettings.getValue("Extruder1DirPin", &dirPin);
  hardwareSettings.getValue("Extruder1StepsPerRevolution", &stepsPerRevolution);
  hardwareSettings.getValue("Extruder1StepsPerMM", &stepsPerMM);
  hardwareSettings.getValue("IsExtruder1Inverted", &axisInversion);
  int  heaterPin, thermisterPin;
  hardwareSettings.getValue("Extruder1StandardSpeed", &standardSpeed);
  hardwareSettings.getValue("Extruder1HeaterPin", &heaterPin);
  hardwareSettings.getValue("Extruder1ThermisterPin", &thermisterPin);
  cout << stepPin << " " << dirPin << " " << stepsPerRevolution << " " << stepsPerMM << " " << axisInversion << " " << standardSpeed << " " << heaterPin << " " << thermisterPin << endl;
  extruder1 = new Extruder(stepPin, dirPin, stepsPerRevolution, stepsPerMM, standardSpeed, axisInversion,heaterPin, thermisterPin);

  //z axis offset
  float tempZOffset;
  float tempRaise;
  hardwareSettings.getValue("ZLimitOffset", &tempZOffset);
  hardwareSettings.getValue("ZRaiseBeforeReset", &tempRaise);
  zAxisOffset = tempZOffset;
  zRaiseBeforeReset = tempRaise;
  //get all max speeds
  hardwareSettings.getValue("XMaxSpeed",&maxSpeed.x);
  hardwareSettings.getValue("YMaxSpeed",&maxSpeed.y);
  hardwareSettings.getValue("ZMaxSpeed",&maxSpeed.z);


}
